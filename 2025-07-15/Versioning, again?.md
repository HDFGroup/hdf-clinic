# HDF5 Versioning, Again?

The following discussion is **not** about versioning the data in an HDF5 from a user's perspective.
For that, see a previous episode on [Versioned HDF5](https://www.youtube.com/watch?v=8unCINLhNeM) and a presentation on the [Onion VFD at HUG25](https://youtu.be/Z3qUenGC-OQ).
This is about versioning in the context of the HDF5 library and file format.

In software, versioning helps us to manage the *conflict* between innovation and compatibility.

> The heart of self-describing data is marriage of a format and
an access library. (Jeffery R. Kuehn)

There is no direct correlation or causation between innovation in the library and the file format. That's why, speaking generally, we version them separately.
Also, applications might access HDF5 files directly without the HDF5 library.

**Note:** Often, versions are (sequences of) numbers. However, other unique identifiers are available to identify versions.

The concept of *compatibility* "entangles" versioning of the library and the file format. 

[Figure]

**Backward compatibility:** The latest HDF5 library version can read HDF5 files created with any earlier library version.

**Forward compatibility:** An earlier version of the HDF library can read HDF5 files created by a later HDF5 library version if the file does not contain on-disk features that were introduced after the earlier version.

**Maximum compatibility principle:** When writing data, the library will automatically use the earliest possible version of the file format and its constituent objects that can fully describe the information being stored.

## The version of the HDF5 library is what it says on the tin

At compile time, the header file `H5public.h` defines several C preprocessor macros that expose the library version:
- `H5_VERS_MAJOR`: The major version number.
- `H5_VERS_MINOR`: The minor version number.
- `H5_VERS_RELEASE`: The release number.
- `H5_VERS_INFO`: A string containing the full version information

### API (Application Programming Interface) compatibility

- Source-level compatibility, i.e., if an API is compatible, code written against an older version of the library can be recompiled with a newer version without source code changes.
- See API compatibility macros

### ABI (Application Binary Interface) compatibility

- Binary-level compatibility, i.e., if an ABI is compatible, an application compiled against an older version of the library can be run with a newer version of the shared library without being recompiled.
- `MINOR` updates are **not** ABI compatible

## The version of the HDF5 file format is more complicated...

### HDF5 file format specification versions

## Recommendations

Use common sense.

### Software Developers
1. The choice of HDF5 library version should be a deliberate architectural decision, not an afterthought.
2. Make an explicit compatibility choice.
3. Enforce compatibility programmatically: use the `H5Pset_libver_bounds`
4. Migrate incrementally: use the API compatibility macros first

### Data Curators and Archivists
1. Mandate a metadata provenance policy
2. Standardize on a long-term support (LTS) format

### System Administrators and HPC Center Staff
1. Provide multiple HDF5 versions
2. Use environment modules or containers

## Questions

1. I'm a developer of an independent HDF5 implementation, and compatibility is not a concern for me. What is the "best" HDF5 file format version? (I'm unsure.)
2. What, if anything, changes in HDF5 2.0.0 concerning versioning? (Ask Scot!)
3. Is there light at the end of the tunnel? ([Yes!](https://www.youtube.com/watch?v=EgtAiYslNGg))
