# From Java Woes to AI Flows: The HDF5 2.0 Direction

## Abstract

About four months ago, I was lamenting the state of HDFView and the poor Java developer experience with the HDF5 library. With the help of AI, my colleague Allen Byrne has made significant strides, including:

- [Maven](https://maven.apache.org/index.html) integration for the HDF5 library Java bindings (JNI-based)
- Maven integration for the HDF5 library Java bindings (Foreign Function and Memory - FFM-based)
- HDFView builds against Maven packages

I will give a brief update on Allen’s achievements. How much of this will show up in HDF5 2.0.0 remains to be seen, but the direction of travel is clear: HDF5 2.0 will be open for AI-assisted development, documentation, and tools.

Secondly, I would like to give you a preview of the work we will do to enhance the safety, security, and privacy (SSP) of HDF5 over the next two years and invite you to join this effort.

## HDF5 library Java bindings

Scot Breitenfeld announced Allen's [pull request](https://github.com/HDFGroup/hdf5/pull/5847) in mid-September on the [HDF forum](https://forum.hdfgroup.org/t/help-needed-reviewing-hdf5-java-maven-integration-pr-5847/13516), asking for help with the review.
(How many people took notice? 74 views so far.)

**What's in it?**

"... implements a comprehensive HDF5 Java Library and Examples Maven integration with:

- [x] Complete Maven Pipeline - All 62 Java examples as deployable Maven artifact
- [x] Cross-Platform CI/CD - Linux, Windows, macOS x86_64, macOS aarch64
- [x] Production Deployment - GitHub Packages integration with version conflict resolution
- [x] Comprehensive Documentation - User guides, technical docs, and status reports
- [x] Fork-Based Testing - Safe validation methodology before canonical deployment

📊 Final Status:

- Implementation: 100% Complete
- Testing: 100% Complete with snapshot versioning
- Documentation: Comprehensive and production-ready

The Maven integration is production-ready and provides HDF5 Java developers with a modern, professional-grade experience using standard Maven dependency management."

You can check it out by simply creating a GitHub Codespace (in a browser). `Code -> Codespaces -> + New with options`

### Extra treat: `CLAUDE.md`

Allen used [Claude Code](https://www.claude.com/product/claude-code) while working on the PR. This is a convenient way for agents to get their bearings, and for developers to get started. There will be a tool-independend way, but it's `CLAUDE.md` for now.

You can tell VSCode about the instructions in this file by adding it to the **Chat: Instructions File Locations** setting. That way it will be available, for example, in Copilot chat.

![Figure](./copilit-chat.png)

If you are a Java developer, the HDF5 library, instead of being a thorn in your side, will soon behave like any other dependence you can download from your favorite Maven repository.

Which brings us to the next topic: *Why should you trust our or anyone else's artifacts?*

## NSF Safe-OSE
