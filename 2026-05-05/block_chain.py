'''
A simple HDF5-backed blockchain implementation.

  ┌─────────────────────┐      ┌─────────────────────┐      ┌─────────────────────┐                                     
  │      Block 0        │      │      Block 1        │      │      Block 2        │                                     
  │     (Genesis)       │      │                     │      │                     │                                     
  ├─────────────────────┤      ├─────────────────────┤      ├─────────────────────┤                                     
  │ index:    0         │      │ index:    1         │      │ index:    2         │                                     
  │ time:     t0        │      │ time:     t1        │      │ time:     t2        │                                     
  │ data:    "alpha"    │      │ data:    "bravo"    │      │ data:    "charlie"  │                                     
  │ prev:    0x0000…    │      │ prev:    0xA1B2…    │◀─┐   │ prev:    0xC3D4…    │◀─┐                                  
  │ hash:    0xA1B2…    │──┐   │ hash:    0xC3D4…    │  │   │ hash:    0xE5F6…    │  │                                  
  └─────────────────────┘  │   └─────────────────────┘  │   └─────────────────────┘  │                                  
                           │                            │                            │                                  
                           └────────────────────────────┘                            │                                  
                                                                                     │                                  
                                                                         head/tip ───┘

This gives you tamper-evident ("was this changed?") structure, NOT tamper-proof
("can this be changed?") storage. Someone who can rewrite the HDF5 file can
rewrite all hashes too. To strengthen it, store the final head_hash outside
the file, sign each block hash with a private key, or use HDF5 SWMR/file locking
rules if multiple writers/readers are involved.

Concurrency: this module assumes a single writer. HDF5 file locking (h5py
default) prevents two writers from opening the same file simultaneously.
Multiple readers may open the file in mode='r' when no writer is active.
'''

import hashlib
import json
import time
from dataclasses import dataclass
from typing import Optional

import h5py
import numpy as np


HASH_SIZE = hashlib.sha256().digest_size
HASH_DTYPE = np.dtype(f"V{HASH_SIZE}")
SHA256_TAG = b"SHA-256"
SHA256_TYPE_NAME = b"HashAlgorithm"
ZERO_HASH = bytes(HASH_SIZE)

"""block_length is redundant but convenient. This could be a smaller unsigned type,
   if blocks are known not to exceed a certain size.
"""
BLOCK_INDEX_DTYPE = np.dtype([
    ("timestamp_ns", "<i8"),
    ("block_offset", "<u8"),
    ("block_length", "<u8"),
])

SCHEMA_NAME = "blockchain.v1"
SCHEMA_NAME_BYTES = SCHEMA_NAME.encode("ascii")
SCHEMA_DTYPE = h5py.string_dtype(encoding="ascii", length=len(SCHEMA_NAME_BYTES))
SCHEMA_VERSION = 1
HASH_ALGO_NAME = "SHA-256"
HASH_ALGO_NAME_BYTES = HASH_ALGO_NAME.encode("ascii")
HASH_ALGO_DTYPE = h5py.string_dtype(encoding="ascii", length=len(HASH_ALGO_NAME_BYTES))
DOMAIN_TAG = b"blockchain.v1\x00"


@dataclass(frozen=True)
class Block:
    index: int
    timestamp_ns: int
    hash: bytes
    payload_bytes: bytes


@dataclass(frozen=True)
class VerifyResult:
    ok: bool
    failed_index: Optional[int] = None
    reason: Optional[str] = None


def create_sha256_type():
    """Return an HDF5 opaque datatype for raw SHA-256 digests."""
    sha256_type = h5py.h5t.create(h5py.h5t.OPAQUE, HASH_SIZE)
    sha256_type.set_tag(SHA256_TAG)
    return sha256_type


def commit_sha256_type(group):
    """Commit the SHA-256 datatype in an HDF5 group."""
    sha256_type = create_sha256_type()
    sha256_type.commit(group.id, SHA256_TYPE_NAME)
    return sha256_type


def open_sha256_type(group):
    """Open the committed SHA-256 datatype from an HDF5 group."""
    return h5py.h5t.open(group.id, SHA256_TYPE_NAME)


def hash_to_array(digest):
    """Return a one-element array containing a SHA-256 opaque value."""
    if len(digest) != HASH_SIZE:
        raise ValueError(f"expected {HASH_SIZE} hash bytes, got {len(digest)}")
    return np.array([np.void(digest)], dtype=HASH_DTYPE)


def hash_to_bytes(value):
    """Return raw SHA-256 digest bytes from an HDF5 opaque value."""
    digest = bytes(value)
    if len(digest) != HASH_SIZE:
        raise ValueError(f"expected {HASH_SIZE} hash bytes, got {len(digest)}")
    return digest


def write_head_hash_attr(group, digest, sha256_type):
    """Write the head_hash attribute as an opaque hash_algo value."""
    name = b"head_hash"
    if "head_hash" in group.attrs:
        attr = h5py.h5a.open(group.id, name)
    else:
        space = h5py.h5s.create(h5py.h5s.SCALAR)
        attr = h5py.h5a.create(group.id, name, sha256_type, space)
    data = np.zeros((), dtype=HASH_DTYPE)
    data[()] = np.void(digest)
    attr.write(data, mtype=sha256_type)


def read_head_hash_attr(group, sha256_type):
    """Read the head_hash attribute and return the raw SHA-256 digest bytes."""
    attr = h5py.h5a.open(group.id, b"head_hash")
    data = np.zeros((), dtype=HASH_DTYPE)
    attr.read(data, mtype=sha256_type)
    return bytes(data.tobytes())


def write_hash(dataset, index, digest, sha256_type):
    """Write one SHA-256 digest into a one-dimensional opaque dataset."""
    file_space = dataset.id.get_space()
    file_space.select_hyperslab((index,), (1,))
    mem_space = h5py.h5s.create_simple((1,))
    dataset.id.write(mem_space, file_space, hash_to_array(digest), mtype=sha256_type)


def read_hash(dataset, index, sha256_type):
    """Read one SHA-256 digest from a one-dimensional opaque dataset."""
    data = np.empty((1,), dtype=HASH_DTYPE)
    file_space = dataset.id.get_space()
    file_space.select_hyperslab((index,), (1,))
    mem_space = h5py.h5s.create_simple((1,))
    dataset.id.read(mem_space, file_space, data, mtype=sha256_type)
    return hash_to_bytes(data[0])


def ensure_capacity(dataset, needed, *, min_growth=16):
    """Resize a 1-D dataset so its shape is at least `needed`, doubling when it must grow."""
    current = dataset.shape[0]
    if needed <= current:
        return
    target = max(needed, current * 2, min_growth)
    dataset.resize((target,))


def canonical_block_bytes(index, timestamp_ns, prev_hash, payload_bytes):
    """Return the canonical bytes that are committed by the block hash."""
    header = {
        "schema": SCHEMA_NAME,
        "schema_version": SCHEMA_VERSION,
        "index": index,
        "timestamp_ns": timestamp_ns,
        "prev_hash": prev_hash.hex(),
        "payload_sha256": hashlib.sha256(payload_bytes).hexdigest(),
    }
    return json.dumps(header, sort_keys=True, separators=(",", ":")).encode()


def block_hash(index, timestamp_ns, prev_hash, payload_bytes):
    """Compute the SHA-256 hash for a block, with a domain-separation prefix."""
    digest = hashlib.sha256()
    digest.update(DOMAIN_TAG)
    digest.update(canonical_block_bytes(index, timestamp_ns, prev_hash, payload_bytes))
    return digest.digest()


class Chain:
    """HDF5-backed blockchain with single-writer semantics.

    Use ``Chain.create(path)`` to create a new file and ``Chain.open(path, mode)``
    to open an existing one. Treat instances as context managers::

        with Chain.create(path) as chain:
            chain.append({"event": "hello"})

        with Chain.open(path) as chain:           # mode='r' default
            for block in chain:
                ...

    Concurrency: HDF5 file locking (h5py default) prevents two writers. Readers
    can open with ``mode='r'`` when no writer is active.
    """

    def __init__(self, h5file):
        self._file = h5file
        self._chain = h5file["chain"]
        self._sha256_type = open_sha256_type(self._chain)
        self._check_schema()

    @classmethod
    def create(cls, path):
        """Create a new chain file. Returns a Chain open in write mode."""
        h5file = h5py.File(path, "w")
        try:
            chain = h5file.create_group("chain")
            sha256_type = commit_sha256_type(chain)
            chain.attrs.create("schema", SCHEMA_NAME_BYTES, dtype=SCHEMA_DTYPE)
            chain.attrs["schema_version"] = np.uint64(SCHEMA_VERSION)
            chain.attrs.create("hash_algo", HASH_ALGO_NAME_BYTES, dtype=HASH_ALGO_DTYPE)
            chain.attrs["n_blocks"] = np.uint64(0)
            chain.attrs["byte_length"] = np.uint64(0)
            write_head_hash_attr(chain, ZERO_HASH, sha256_type)
            chain.create_dataset(
                "block_index", shape=(0,), maxshape=(None,),
                dtype=BLOCK_INDEX_DTYPE, compression="gzip",
            )
            chain.create_dataset(
                "hash", shape=(0,), maxshape=(None,), dtype=sha256_type,
            )
            chain.create_dataset(
                "block_bytes", shape=(0,), maxshape=(None,),
                dtype=np.uint8, chunks=(1 << 20,), compression="gzip",
            )
            h5file.flush()
            return cls(h5file)
        except Exception:
            h5file.close()
            raise

    @classmethod
    def open(cls, path, mode="r"):
        """Open an existing chain. mode='r' for readers, mode='a' for the writer."""
        h5file = h5py.File(path, mode)
        try:
            return cls(h5file)
        except Exception:
            h5file.close()
            raise

    def _check_schema(self):
        attrs = self._chain.attrs
        if "schema_version" not in attrs:
            raise ValueError("file has no schema_version attribute; not a Chain v1 file")
        version = int(attrs["schema_version"])
        if version != SCHEMA_VERSION:
            raise ValueError(
                f"unsupported schema_version {version} (this module is v{SCHEMA_VERSION})"
            )
        algo = attrs.get("hash_algo", b"")
        if isinstance(algo, (bytes, np.bytes_)):
            algo = algo.decode("ascii")
        if algo != HASH_ALGO_NAME:
            raise ValueError(f"unsupported hash_algo {algo!r} (expected {HASH_ALGO_NAME!r})")

    def close(self):
        self._file.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        self.close()
        return False

    def __len__(self):
        return int(self._chain.attrs["n_blocks"])

    @property
    def head_hash(self):
        """Return the SHA-256 of the latest block, or ZERO_HASH if empty."""
        return read_head_hash_attr(self._chain, self._sha256_type)

    def append(self, payload):
        """Append a block. `payload` may be bytes or a JSON-serializable object.

        Returns the new block's hex digest. The visibility marker (``n_blocks``)
        is bumped only after the payload, hash, and ``byte_length`` are flushed,
        so a crash during append leaves the new block invisible to readers.
        """
        if isinstance(payload, (bytes, bytearray)):
            payload_bytes = bytes(payload)
        else:
            payload_bytes = json.dumps(payload, sort_keys=True).encode()

        chain = self._chain
        sha256_type = self._sha256_type

        index = int(chain.attrs["n_blocks"])
        byte_offset = int(chain.attrs["byte_length"])
        length = len(payload_bytes)
        byte_end = byte_offset + length

        if index == 0:
            prev_hash = ZERO_HASH
        else:
            prev_hash = read_hash(chain["hash"], index - 1, sha256_type)

        timestamp_ns = time.time_ns()
        digest = block_hash(index, timestamp_ns, prev_hash, payload_bytes)

        for name in ("block_index", "hash"):
            ensure_capacity(chain[name], index + 1)
        ensure_capacity(chain["block_bytes"], byte_end, min_growth=1 << 20)

        chain["block_index"][index] = (timestamp_ns, byte_offset, length)
        write_hash(chain["hash"], index, digest, sha256_type)
        chain["block_bytes"][byte_offset:byte_end] = np.frombuffer(payload_bytes, dtype=np.uint8)
        chain.attrs["byte_length"] = np.uint64(byte_end)

        self._file.flush()
        chain.attrs["n_blocks"] = np.uint64(index + 1)
        write_head_hash_attr(chain, digest, sha256_type)
        self._file.flush()

        return digest.hex()

    def __getitem__(self, index):
        chain = self._chain
        n = int(chain.attrs["n_blocks"])
        if index < 0:
            index += n
        if index < 0 or index >= n:
            raise IndexError(f"block index {index} out of range [0, {n})")
        entry = chain["block_index"][index]
        offset = int(entry["block_offset"])
        length = int(entry["block_length"])
        return Block(
            index=index,
            timestamp_ns=int(entry["timestamp_ns"]),
            hash=read_hash(chain["hash"], index, self._sha256_type),
            payload_bytes=bytes(chain["block_bytes"][offset:offset + length]),
        )

    def __iter__(self):
        chain = self._chain
        n = int(chain.attrs["n_blocks"])
        if n == 0:
            return
        byte_length = int(chain.attrs["byte_length"])
        block_index = chain["block_index"][:n]
        all_bytes = bytes(chain["block_bytes"][:byte_length])
        for i in range(n):
            entry = block_index[i]
            offset = int(entry["block_offset"])
            length = int(entry["block_length"])
            yield Block(
                index=i,
                timestamp_ns=int(entry["timestamp_ns"]),
                hash=read_hash(chain["hash"], i, self._sha256_type),
                payload_bytes=all_bytes[offset:offset + length],
            )

    def verify(self):
        """Verify hash links, payload integrity, timestamp monotonicity, head_hash."""
        chain = self._chain
        sha256_type = self._sha256_type
        n = int(chain.attrs["n_blocks"])
        byte_length = int(chain.attrs["byte_length"])
        head_hash_attr = read_head_hash_attr(chain, sha256_type)

        if n == 0:
            if head_hash_attr != ZERO_HASH:
                return VerifyResult(False, None, "head_hash attr is non-zero on an empty chain")
            return VerifyResult(ok=True)

        block_index = chain["block_index"][:n]
        all_bytes = bytes(chain["block_bytes"][:byte_length])

        prev_hash = ZERO_HASH
        prev_ts = None

        for index in range(n):
            entry = block_index[index]
            byte_offset = int(entry["block_offset"])
            length = int(entry["block_length"])
            byte_end = byte_offset + length
            timestamp_ns = int(entry["timestamp_ns"])

            if byte_offset < 0 or length < 0 or byte_end > byte_length:
                return VerifyResult(
                    ok=False,
                    failed_index=index,
                    reason=f"payload range out of bounds (offset={byte_offset}, length={length}, byte_length={byte_length})",
                )

            if prev_ts is not None and timestamp_ns < prev_ts:
                return VerifyResult(
                    ok=False,
                    failed_index=index,
                    reason=f"timestamp regression ({timestamp_ns} < {prev_ts})",
                )

            payload_bytes = all_bytes[byte_offset:byte_end]
            stored = read_hash(chain["hash"], index, sha256_type)
            expected = block_hash(index, timestamp_ns, prev_hash, payload_bytes)

            if stored != expected:
                return VerifyResult(
                    ok=False,
                    failed_index=index,
                    reason="block hash does not match recomputed hash",
                )

            prev_hash = expected
            prev_ts = timestamp_ns

        if head_hash_attr != prev_hash:
            return VerifyResult(
                ok=False,
                failed_index=n - 1,
                reason="head_hash attr does not match last block's hash",
            )

        return VerifyResult(ok=True)


def init_chain(path):
    """Create a new empty HDF5-backed block chain."""
    Chain.create(path).close()


def append_block(path, payload):
    """Append a JSON-serializable payload as the next block."""
    with Chain.open(path, mode="a") as chain:
        return chain.append(payload)


def verify_chain(path):
    """Verify the chain at `path` and return a VerifyResult."""
    with Chain.open(path, mode="r") as chain:
        return chain.verify()


def get_block(path, index):
    """Return the block at `index` as a Block."""
    with Chain.open(path, mode="r") as chain:
        return chain[index]


def iter_blocks(path):
    """Yield every block in order."""
    chain = Chain.open(path, mode="r")
    try:
        yield from chain
    finally:
        chain.close()


if __name__ == "__main__":
    filename = "chain.h5"

    with Chain.create(filename) as chain:
        chain.append({"event": "created", "value": 1})
        chain.append({"event": "updated", "value": 2})
        chain.append(b"\x00\x01\x02\x03\x04")
        chain.append(bytes(range(32)))
        chain.append(bytearray(b"raw bytes payload"))
        chain.append({"event": "updated", "value1": 1, "value2": 2})
        chain.append({"event": "deleted", "reason": "no longer needed"})
        chain.append({"event": "created", "value": 3})

    with Chain.open(filename) as chain:
        result = chain.verify()
        if result.ok:
            head = chain.head_hash.hex()
            print(f"{filename} verified: True ({len(chain)} blocks, head={head[:16]}...)")
        else:
            print(f"{filename} verified: False (block {result.failed_index}: {result.reason})")
        for block in chain:
            try:
                shown = block.payload_bytes.decode()
            except UnicodeDecodeError:
                shown = block.payload_bytes.hex()
            print(f"  block {block.index}: {shown}")
