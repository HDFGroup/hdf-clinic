# A Simple VectorDB in HDF5

We will:

- Do semantic search over Shakespeare in one HDF5 file
- Define "vector database" (in plain English)
- Walk through the build + query graph
- Discuss easy upgrades (speed, quality, UX)

## What we'll cover

- Vector DB 101: embeddings + similarity + metadata​
- The build pipeline: download → split → chunk → embed → write HDF5​
- The on-disk layout: groups, datasets, and a “works” index​
- Querying: cosine similarity via blockwise dot products​
- Improvements: faster search, richer metadata, better chunking, reproducibility​

## Vector database: the idea

A “vector database” stores:​

- Vector embeddings​
- Metadata (titles, ids, filters, offsets…)​
- An index for similarity search (optional, but common)​

So you can ask:

> "Find passages most similar to this query text."

Optionally: "...within Macbeth" (metadata filter)

## Embeddings + cosine similarity

*"to be or not to be"* → 384-D embedding vector ([`all-MiniLM-L6-v2`](https://huggingface.co/sentence-transformers/all-MiniLM-L6-v2))

**Similarity (cosine):**
```
cos(u,v) = (u · v) / (||u|| ||v||)
```
If embeddings are unit‑normalized, cosine similarity is just a dot product.​ (That’s why the script stores normalized vectors and uses `M @ q`.)

## Why put vectors in HDF5?

- Portable single file (easy to share)​
- Hierarchical layout: groups + datasets + attributes​
- Chunking + compression for large arrays​
- Great tooling ecosystem (`h5ls`, `h5dump`, `H5Web`, `h5py`)​

### Tradeoffs

- Not a multi-user database server​ (see HSDS for that!)​
- No built-in Approximate Nearest Neighbors (ANN) index (we add one if needed)​
- Schema discipline is on you​

## How the script builds the "DB"

1. Download Gutenberg
2. Split into  works
3. Token chunking
4. Embed chunks
5. Write HDF5

The output is just arrays + a small index table. That’s enough for semantic search.​

```
python shakespeare_hdf5_vectors.py build --out shakespeare.h5
```

## Split and chunk the corpus

1. Split into works
   - Detect play headings (ALL CAPS).​
   - Special-case “SONNETS”: split into 154 poems.​
   - Store a compact “works index” with row ranges.​
2. Token-based chunking
   - `max_tokens=256, overlap=32 (BERT tokenizer)​`
   - Why overlap? To avoid dropping meaning at chunk boundaries.​

**Key loop (simplified):**
```python
1 ids = tokenizer.encode(text, add_special_tokens=False)​
2 step = max_tokens - overlap​
3 for start in range(0, len(ids), step):​
4     window = ids[start:start+max_tokens]​
5     chunk = tokenizer.decode(window)​
6     chunks.append(chunk.strip())
```

## On-disk schema

Minimal schema: arrays + index table​
```
/meta (attrs: created_at, source, tokenizer, embedder, …)​

/chunks​
       text : [N] vlen utf-8​
    work_id : [N] int32 (gzip)​
  embedding : [N, D] float32 (gzip, unit-normalized)​

/works​
      index : [W] compound {work_id, title, start_row, end_row}​
```
Think: “a column store for vectors + a lookup table for metadata filters.”

## Querying the HDF5 "vector DB"

### Query steps

- Embed the query text → `q` (unit vector)​
- Optional work filter → row range [lo, hi)​
- Blockwise dot products: `score = M @ q​`
- Pick top‑k via `argpartition` + `sort`​
- Return chunk text + metadata​

### Complexity

This demo does a linear scan(m?): `O(N · D)` per query

Fine for “clinic-sized” corpora. For millions of vectors, add an ANN index.​

(Implementation detail:​ Scores computed in blocks to avoid loading all embeddings at once.)

## Demo: buid , inspect, query

Try these commands at home:

```bash
1 python shakespeare_hdf5_vectors.py build --out shakespeare.h5​
2 python shakespeare_hdf5_vectors.py list-works --h5 shakespeare.h5​
3 python shakespeare_hdf5_vectors.py query --h5 shakespeare.h5 --text "dagger" --work "Macbeth" --top-k 5​
4 python shakespeare_hdf5_vectors.py query --h5 shakespeare.h5 --text "to be or not to be" --top-k 5​
5 python shakespeare_hdf5_vectors.py query --h5 shakespeare.h5 --text "witches around cauldron" --top-k 5​
```
**Tip:** open the file in an HDF5 browser ([H5Web](https://h5web.panosc.eu/)) to show the schema visually.

## How to improve this “vector DB”

### Performance

- Add ANN indexing (Hierarchical navigable small world (HNSW) / Facebook AI Similarity Search (FAISS))​
- Quantize vectors (`float16` / `int8`)​
- Tune dataset chunk shapes for read patterns​
- Parallelize embedding + search​

### Quality

- Use the embedder’s tokenizer for chunking​
- Experiment: chunk sizes, overlap, separators​
- Store offsets → show more context around hits​
- Optional reranker (cross-encoder) for top results​

### Usability

- Richer metadata: play / act / scene / line​
- CRUD: incremental updates + versioning​
- Schema docs + validation (attrs + checks)​
- Hybrid search: keyword filter + vector search​

## Takeaways

**In 20 minutes we built:**

- A minimal vector store (embeddings + text)​
- Metadata for filtering (works index → row ranges)​
- A query path (cosine ≈ dot for normalized vectors)​
- A clear upgrade path (ANN, richer schema, better chunking)​

**Questions? Want to turn this into a reusable pattern for your datasets? Agentic HDF5?​**

