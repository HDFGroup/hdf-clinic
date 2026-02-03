# SBOMs & Sunshine: The HDF5 SHINES Check-In

## Software Bill of Materials (SBOM)

![Ingredients](./OIP.jpeg)
![Bob](./bob.jpg)

### References

- [Framing Software Component Transparency: Establishing a Common Software Bill of Materials (SBOM)](https://www.cisa.gov/sites/default/files/2024-10/SBOM%20Framing%20Software%20Component%20Transparency%202024.pdf)
- [How to create SBOMs in Java with Maven and Gradle](https://snyk.io/blog/create-sboms-java-maven-gradle/)
- [Automating creation of Software Bills of Materials](https://archive.fosdem.org/2021/schedule/event/automating_creation_of_spdx_sbom/)
- [Complete Guide to C/C++ SBOM Generation](https://sbomgenerator.com/guides/cpp)
- [Spack SBOM example](https://github.com/spack/spack-sbom/blob/1b16b6ed9f2c71fd9ff71951433b73d39f4650a5/examples/zlib.json)

### HDF5 build process intricacies

- Who is the audience? (integrators, package maintainers, etc.)
- Modularity is key
- Gazillion of CMake options
  - Ideally CMake would take care of this, but ...   
- Currently looking at [CycloneDX](https://cyclonedx.org/)
- Check [work in progress](https://github.com/HDFGroup/hdf5-ssp-sig/tree/main/templates/sbom)
- Build phases
  - `pre-build` => HDF5 source tarball
  - `build` => Build options (CMake configuration)
  - `post-build` => binaries
- During `build`: core, addons (filters, VFD), tools, HL interfaces
- During `post-build`: plugins, bundles 

## First audit

## HDF5 Safety, Security, & Privacy (SSP) Special Interest Group (SIG)

## HDF5 Product Security Incident Response Team (PSIRT)

## Getting involved

## Acknowledgement

> This material is based upon work supported by the U.S. National Science Foundation under Federal Award No. 2534078. Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
