# HDF5 and Open Science

## Open Science

Wet (liquid) water? IMHO, it's a misnomer: it puts the cart before the horse by emphasizing the means (open practices; see [4]) rather than the end (reinventing discovery; see [1]).

## HDF5's Key Contributions to Open Science

At its core, HDF5 is about sharing data and assuring access across platforms and environments, indefinitely or for the lifetime of the data, regardless of size or complexity. In practice, we achieved this by proposing a simple data model first implemented as an open file format and a supporting open-source software library and, later, as a RESTful web service. HDF5's key contributions to Open Science can be summarized as follows: 

1. **Interoperability and Standardization:** HDF5 provides a well-documented, standardized file format that can be used across different computing platforms, programming languages (C, C++, C#, Fortran, Python, R, etc.), and operating systems. Ensuring that data stored in HDF5 can be read and manipulated reliably across diverse environments reduces technical barriers to data sharing. 
2. **Efficient Handling of Large, Complex Data:** HDF5 is designed to efficiently store and access massive datasets (terabytes to petabytes). Its hierarchical structure (groups and datasets) allows researchers to organize complex data and metadata in a single file, streamlining analysis and sharing. This capacity for handling large, multidimensional data arrays is especially important in fields such as physics, climate science, genomics, and astronomy. 
3. **Open-Source Ecosystem:** HDF5 libraries and tools are distributed under a permissive open-source license, encouraging community contributions and widespread adoption. An ecosystem of open-source tools (e.g., h5py for Python, rhdf5 for R) extends HDF5’s functionality, making it easier for researchers to integrate HDF5 into their workflows. 
4. **Reproducibility and Transparency:** By providing a consistent way to package raw data, metadata, and provenance information, HDF5 helps ensure that research can be reproduced. Well-structured HDF5 files facilitate clear documentation of how data were generated, processed, and analyzed, supporting the reproducibility goals of Open Science. 
5. **Long-Term Data Preservation:** HDF5 was created with data longevity in mind, offering forward- and backward compatibility of files, version tracking, and stable APIs. Its self-describing nature means that future researchers can more easily interpret HDF5 files—even if the original software environment no longer exists. 
6. **Broad Adoption and Community Support:** HDF5 is widely used by major scientific institutions (NASA, DOE, etc.) and large-scale experiments (e.g., astronomy surveys, climate modeling), strengthening its standing as a robust choice for data-intensive research. Active user communities, including The HDF Group and open-source contributors, provide ongoing support, documentation, and feature development. 

Overall, HDF5’s open-source model, robust technical capabilities, and commitment to interoperability make it a foundational tool for researchers looking to share, preserve, and analyze data in a transparent and reproducible manner. This positions HDF5 as a critical enabler of Open Science practices, where the free data exchange is key to accelerating scientific discovery. 

## Open Science is Networked Science

When people say that *"HDF5 data is available on the network,"* it can mean a few different things depending on context, architecture, and usage scenario. Here are a few examples: 

- **Remote File Access (e.g., Network File Systems):** An HDF5 file is simply located on a network‐accessible file system (like NFS, SMB, or a parallel file system such as Lustre) and is being opened by processes running on different machines. Multiple remote clients can open and read/write the same .h5 file if the file system and HDF5 library settings support concurrency. The file may be behind enterprise authentication or only accessible within certain subnets. Despite being “on the network,” it may not be publicly available—access can still be heavily restricted. 
- **HDF5 Data Services or “HDF Server”:** The HDF5 data is served via a specialized service (e.g., HDF Server or HSDS from The HDF Group). These services provide a REST, HTTP, or similar API allowing remote clients to query or modify dataset contents without direct file access. Clients do not open .h5 files directly; rather, they make requests to the server, which internally handles the HDF5 data layout. This approach can be more secure than direct file sharing since the server can enforce authentication, fine‐grained authorization, or rate limits. It facilitates cloud‐based architectures where HDF5 data is stored in an object store (e.g., AWS S3) and exposed through an HTTP interface. 
- **Public/Shared Data Repositories:** The HDF5 file is placed in a data repository or platform (e.g., Zenodo, Figshare, an institutional data portal) that makes it publicly available over the internet. Interested users can download the .h5 file or subsets of its contents. Anyone can retrieve a complete copy of the HDF5 file. Researchers often use this to disseminate large datasets for reproducibility. License agreements might control access and usage, but technically, it’s still a direct download link. 
- **Cloud‐Native Object Stores:** HDF5 structures (e.g., chunked data, metadata) may be stored in a cloud object store (e.g., AWS S3, Azure Blob Storage, or Google Cloud Storage). Users or applications then read data objects remotely from cloud-optimized HDF5 files or, sometimes, using libraries tailored for cloud read/write operations (e.g., h5pyd with HSDS). To avoid downloading entire files, data can be distributed across many storage nodes, with partial or “lazy” reads. Performance depends on the I/O patterns and network bandwidth/latency. Authentication and encryption are usually handled at the cloud service layer.

> While there seems to be no shortage of technical solutions for different use cases and environments, they all share a common problem: the _"Internet without search engines and semantics"_ problem. The data is neatly stored in disconnected silos that are known only to insiders, usable for specific purposes only, and no machine-readable metadata is on hand. 

## Complementing HDF5 with Knowledge Graphs (KG)

Knowledge graphs play an increasingly important role in Open Science by providing a structured way to organize, link, and integrate research data, publications, and other scholarly resources across different domains and institutions. By representing complex relationships in a graph structure—nodes (entities) and edges (relationships)—knowledge graphs make scientific information more discoverable, interoperable, and reusable. In essence, knowledge graphs are a powerful tool for organizing and interlinking the vast and heterogeneous body of scientific knowledge in an open, machine-readable manner. By doing so, they advance key principles of Open Science: transparency, collaboration, and reusability. As more researchers and institutions embrace knowledge graph technologies, we can expect richer scientific ecosystems where insights emerge more rapidly, reproducibility is strengthened, and innovative research questions find new pathways to answers. 

Combining HDF5 and knowledge graphs can create a powerful ecosystem for storing, organizing, linking, and discovering scientific information. HDF5 excels at efficiently handling large, complex datasets—particularly multidimensional or nested scientific data—while knowledge graphs provide a semantic layer that interlinks datasets, publications, researchers, and other entities in a machine-readable format. Below are a few strategies and benefits for integrating the two to advance Open Science: 

1. **Enhanced Metadata and Provenance Tracking**
    - Rich Metadata in Graph Form:
      - While HDF5 supports metadata at the dataset or file level (e.g., attributes), researchers often need more complex provenance information—like which scripts or parameters generated a dataset, or how multiple datasets relate.
      - A knowledge graph can represent these relationships (e.g., “dataset A derived from dataset B using method M”), offering fine-grained traceability of data transformations.
    - Linking HDF5 Objects to External Entities:
      - Each group or dataset in HDF5 can be assigned a unique identifier (e.g., a URI/IRI) that points to corresponding nodes in a knowledge graph. 
      - By storing these identifiers in the HDF5 file’s attributes, you link the “raw” or “processed” data (in HDF5) to semantic descriptions (in the graph), enabling queries that unify data content with high-level context (authors, institutions, publications). 

2. **Interoperability Across Repositories and Tools**
    - Bridging File-Based and Graph-Based Systems:
      - HDF5 is widely used in fields like physics, climate modeling, and astronomy. Knowledge graphs, meanwhile, are popular for cross-domain linking and advanced semantic queries.
      - By building connectors or mapping layers (e.g., using RDF or JSON-LD) that transform HDF5 metadata to graph nodes and edges, researchers can seamlessly move between file-based data analysis and ontology-driven discovery.
    - Facilitating Multi-Domain Studies: As science becomes more interdisciplinary, the ability to locate and integrate data from different fields is crucial. A knowledge graph that “knows” how HDF5-based observational data in climate science relates to, say, a genomics dataset (also stored in HDF5) can foster cross-domain insights. 

3. **Advanced Search and Discovery**
    - Semantic Search for Large Scientific Datasets:
      - Traditional searches (e.g., searching file names or basic metadata) might not capture the complexity or relationships within HDF5 files.
      - A knowledge graph indexing the metadata and relationships of each HDF5 dataset allows contextual queries: "Find all HDF5 datasets produced by Lab X on Topic Y that used Method Z." This goes beyond keyword matching, leveraging the graph’s linked structure to surface connections or relevant datasets researchers might otherwise miss. 
    - Recommender Systems: Once data are represented in a knowledge graph, machine learning or rule-based engines can recommend related datasets, highlight research overlaps, or identify potential collaborators, accelerating new discoveries and collaborations. 

4. **Facilitating FAIR Data Principles**
    - HDF5’s Role:
       - Ensures Accessibility of large-scale data in a well-documented, widely supported format.
       - Supports Reusability by efficiently packaging data and associated metadata.
    - Knowledge Graph's Role:
      - Improves Findability by exposing data entities and relationships via standard semantic technologies (RDF, SPARQL).
      - Promotes Interoperability by aligning with ontologies and vocabularies shared across the scientific community. 
    - Combined Benefit: When large datasets are stored in HDF5, but described and linked through a knowledge graph, they become more discoverable, interoperable, and reusable—directly supporting Open Science and FAIR practices. 

5. **Reproducibility and Data Provenance**
    - [Capturing the Full Workflow](https://www.protocols.io/): A knowledge graph can document workflow steps, computational pipelines, software versions, and parameter settings. Each step’s output is an HDF5 dataset, explicitly linked via the graph. This makes it easier for others to replay or validate analysis steps, addressing one of the central challenges in Open Science: reproducibility. 
    - Persistent Identifiers (PIDs): Assigning DOIs or other PIDs to HDF5 files, and referencing these in the knowledge graph, provides a persistent link to the data. The knowledge graph can also record versioning information, so if a dataset is updated, the graph maintains a chain of historical references. 

6. **Community Platforms and Tooling**
    - Automated Ingestion: Tools could automatically parse HDF5 files, extract metadata (e.g., dimension sizes, variable names, units), and push these details into a knowledge graph. This streamlines data curation, reducing the manual overhead for researchers to maintain high-quality metadata. 
    - Public Knowledge Graphs: Platforms like Wikidata or Open Research Knowledge Graph (ORKG) could ingest references to large HDF5 datasets, making them more visible and linkable. Researchers worldwide could discover, query, and connect these datasets without needing specialized domain knowledge of their original repository. 

When integrated effectively, they form a powerful infrastructure that enhances data discovery, supports reproducibility, and accelerates collaborative research—all core pillars of Open Science. By adopting this two-pronged approach—storing the data in an open, high-performance format and describing/connecting it via a knowledge graph—researchers can foster more transparent, accessible, and impactful scientific outcomes.

## Use Case

![HDF5 KG RAG](./HDF5%20KG%20RAG.png)

Example: [DOE Water Power Technology Office's (WPTO) US Wave dataset](https://www.wikidata.org/wiki/Q131989949)

## Tools to get started

### Python modules

- `rdflib`
- `requests`
- `openai`
- `SPARQLWrapper`
- `torch`, `torch_geometric`, `sklearn`

### Misc.

- [Wikidata Query Service](https://query.wikidata.org/)
- [Wikidata:Schemas](https://www.wikidata.org/wiki/Wikidata:Schemas)
- [ShEx2 — Simple Online Validator](https://shex-simple.toolforge.org/wikidata/packages/shex-webapp/doc/shex-simple.html)
- [Kaggle](https://www.kaggle.com/search?q=HDF5+sortBy%3Adate)

## Questions?

- [Email me](mailto:gheber@hdfgroup.org) if you want to collaborate on this!
- **ChatGPT update:** Given the BNF representation [*An Example of an HDF5 File in DDL*](https://support.hdfgroup.org/documentation/hdf5/latest/_d_d_l_b_n_f114.html), `ChatGPT o3-mini-high` was able to generate a C program that creates an HDF5 file whose `h5dump` output matches. (It contains only two interesting mistakes: the first VLEN sequence in `/dset3` is empty, and the in-memory datatype for `/group1/dset3` is wrong on little-endian architectures.) 

## References

1. *Reinventing Discovery: The New Era of Networked Science.* By Michael Nielsen, Princeton University Press, 2018.
2. [Wikidata](https://www.wikidata.org/wiki/Wikidata:Main_Page)
3. [Retrieval-augmented generation (Wikipedia)](https://en.wikipedia.org/wiki/Retrieval-augmented_generation)
4. [Open science (Wikipedia)](https://en.wikipedia.org/wiki/Open_science)
5. [SEEKCommons](https://seekcommons.org/)
6. [IOWarp](https://grc.iit.edu/research/projects/iowarp/)
