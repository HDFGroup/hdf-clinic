# Better Tools for HDF5

## Context

(No pun intended)

### [IBM (Open) Data Explorer](https://en.wikipedia.org/wiki/IBM_OpenDXhttps://en.wikipedia.org/wiki/IBM_OpenDX)

> **ChatGPT (4o):** IBM Data Explorer refers to a suite of tools developed by IBM for visualizing and analyzing complex data sets, particularly in scientific and engineering contexts. Originally known as IBM Visualization Data Explorer, it was later released as an open-source project under the name OpenDX.​
>
> **Key Features:** Visual Programming Environment (VPE), Data Model, Modules, Scripting Language, Interactive Visualization

One of its lesser known features (and one of my favorites) was its [Data Prompter](https://sites.cc.gatech.edu/scivis/dx/pages/qikgu030.htm#HDRPROMPT).

**IDEA:** Take you from a blank screen to a visual impression of your data by answering a few questions (dialog boxes), and without writing a single line of code. (1991)

### [HDFView](https://www.hdfgroup.org/download-hdfview/)

> **ChatGPT (4o):** **HDFView**  is a **visualization and editing tool for HDF5 and HDF4 files**, developed and maintained by The HDF Group. It provides a graphical user interface (GUI) that allows users to browse, inspect, and modify the contents of HDF files without writing code.
> 
> ... (key features, technical details, who uses it)

~~LLM~~ => (Large) Visual Language Models! (VLM) 😁

The best HDFView is yet to come...

👉 Don't miss John Readey's *HDF GUI Tool Round Table* at [HUG25](https://www.hdfgroup.org/hug/hug25/)!

### [HDF5 Product Designer](https://www.rd-alliance.org/wp-content/uploads/2024/05/Powers.HDF20Product20Designer20RDA.pdf)

> **ChatGPT (4o):** There is no official role or tool titled **"HDF5 Product Designer"** in the HDF5 ecosystem, so the phrase could mean different things depending on the context. Here are three possible interpretations: ...
>
> ...
>
> **2. A Tool: HDF5 File/Structure Designer**
>
> Some users or organizations refer to custom-built GUI tools (or proposed ones) that help design and visualize HDF5 file structures—e.g., groups, datasets, metadata—without writing code. These tools are sometimes called:
> - "HDF5 Designer"
> - "Data structure builder"
> - "HDF5 schema editor"
>
> While not officially provided by The HDF Group, there are third-party attempts or proposals for such interfaces, especially in domains with non-programmer users.

Good idea(s), just a few years early.

## `README.LLM`

What's the purpose of `README` files?

> **ChatGPT (4o):** The **purpose of README files** is to introduce, explain, and guide users (or developers) about a project, software package, dataset, or any shared resource.
> ... (what makes a good README) ... **A README is your project's front door. It invites users in, tells them what they’ll find inside, and shows them how to get started.**

What's the purpose of `README.LLM` files?

Check out [ReadMe.LLM: A Framework to Help LLMs Understand Your Library](https://arxiv.org/abs/2504.09798)!

What would a `README.LLM` file for the HDF5 library look like?

## A mock `README.LLM` for the HDF5 Library

```
<rules>
- Prefer using official HDF5 library functions over reimplementing functionality manually.
- Avoid hallucinating non-existent functions; if unsure, focus on documented features.
- Follow correct resource management: open files, operate on them, then close them.
- Use appropriate error handling: always check for success/failure where relevant.
- If the file access property list or dataset creation property list is needed, explain why.
</rules>

<description>
HDF5 is a file format and data model designed to store and organize large amounts of complex data. 
The HDF5 library provides APIs for creating, reading, writing, and organizing data in a hierarchical structure (groups, datasets, attributes).
It is widely used in scientific computing, engineering, and big data applications.
Core concepts include datasets (multidimensional arrays), groups (folder-like containers), attributes (metadata), and datatypes.
</description>

<examples>

<example>
<task>Create a new HDF5 file and a dataset.</task>
<code>
import h5py
import numpy as np

# Create a new HDF5 file
with h5py.File('example.h5', 'w') as f:
    data = np.arange(100).reshape((10, 10))
    dset = f.create_dataset('mydataset', data=data)
</code>
</example>

<example>
<task>Read an existing HDF5 dataset.</task>
<code>
import h5py

# Open an existing HDF5 file
with h5py.File('example.h5', 'r') as f:
    dset = f['mydataset']
    data = dset[()]
    print(data)
</code>
</example>

<example>
<task>Add an attribute to a dataset.</task>
<code>
import h5py

with h5py.File('example.h5', 'a') as f:
    dset = f['mydataset']
    dset.attrs['description'] = 'This is a 10x10 array of integers.'
</code>
</example>

<example>
<task>List all datasets in a file recursively.</task>
<code>
import h5py

def list_datasets(name, obj):
    if isinstance(obj, h5py.Dataset):
        print(name)

with h5py.File('example.h5', 'r') as f:
    f.visititems(list_datasets)
</code>
</example>

<example>
<task>Create a compressed dataset.</task>
<code>
import h5py
import numpy as np

data = np.random.random((1000, 1000))

with h5py.File('compressed_example.h5', 'w') as f:
    dset = f.create_dataset('compressed', data=data, compression="gzip", compression_opts=9)
</code>
</example>

</examples>
```

🗝️ **Key points:**

- `<rules>`: Give LLMs clear operational guidelines (e.g., don’t invent functions).
- `<description>`: Provide the basic mental model of what HDF5 is about (datasets, groups, attributes).
- `<examples>`: Multiple common operations with real, executable code.

And that's all there is to it. (Except for arborists, botanists, computer scientists, etc.) Isn't that how we learn what a *tree* is?

## Creating better tools for HDF5

The HDF5 out-of-the-box experience is dismal. There are too many bits and pieces that don't seem to cohere, and look like winners of ugly baby contests.

> “Technically HDF5 is done, except for the parts people actually use.” (Anonymous)

**HDF5 is the most versatile container for sharing scientific and engineering data.** This is achieved by giving people a universal container and the tools to adapt it for their needs. *Why not give them a universal HDF5 tool that they can adapt for their needs?* (Basically, a bunch of `README.LLM` files plus an LLM that can do the plumbing.) HDFView was supposed to be that tool, but technology got in the way. Maybe this is still a bit too ambitious for now, but let's look for opportunities, where we can harness the phenomenal talent in the community AND the power of comrade AI 🤖 bring these disparate pieces together.

## Next time

A recurring theme on our [IOWarp project](https://grc.iit.edu/research/projects/iowarp/) is 'generalization.' Our Materials Science partners at the University of Utah keep brining it up and we keep seeing it at the systems level. As a trained mathematician, I'm more than intrigued and warming up to the idea that LLMs (and graphs!) might be the best tool we have at the moment to make progress in this area.
