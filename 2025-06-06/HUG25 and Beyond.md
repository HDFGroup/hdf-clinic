# HUG25 and Beyond

The 2025 HDF User Group meeting (HUG) just (May 26-28) took place at DESY in Hamburg, Germany.
Thanks to our hosts, sponsors, the Organizing Committee, and to Lori Cooper (HDFG).

The first 13 videos are ready on [YouTube](https://www.youtube.com/watch?v=dyQx-OSnGbk&list=PLPyhR4PdEeGa5xkurCDaLZ5S4ppN_Z86w). Go check them out!

## Day 1 Selection

- [The keys to the management of time-evolution signals on HDF5 files in ITER](https://www.youtube.com/watch?v=emCQM_CtBgQ&list=PLPyhR4PdEeGa5xkurCDaLZ5S4ppN_Z86w&index=3)
  - Probably the heaviest SWMR users on the planet...
- [High Performance Storage for HPC: Proven HDF5 Scalability and Consistency on Pure Storage FlashBlade](https://www.youtube.com/watch?v=YaSZ4hA1mvE&list=PLPyhR4PdEeGa5xkurCDaLZ5S4ppN_Z86w&index=5)
  - Yes, you can SWMR with NFS!
  - Do you really need a parallel file system?
- [Open HDF5-Based Format for Industrial Inspection Data w/Exp Integration of the Onion VFD](https://www.youtube.com/watch?v=Z3qUenGC-OQ&list=PLPyhR4PdEeGa5xkurCDaLZ5S4ppN_Z86w&index=8)
  - We talked about [Versioned HDF5](https://github.com/HDFGroup/hdf-clinic/tree/main/2024-10-29)
  - HDF5 file versioning is a true "diamond in the rough"
  - Let's finish the job!
- [Using a HDF5 File as a Zarr v3 Shard](https://www.youtube.com/watch?v=c4b_yfIeHJc&list=PLPyhR4PdEeGa5xkurCDaLZ5S4ppN_Z86w&index=9)
  - Mark's got an excellent point, and we try to be helpful
  - But *"For Heaven's sake, do not confound HDF5 with anything else!"*
—Friedrich Nietzsche, Ecce Homo
- [Down to the bytes: can we simplify alternative access to HDF5?](https://www.youtube.com/watch?v=EgtAiYslNGg&list=PLPyhR4PdEeGa5xkurCDaLZ5S4ppN_Z86w&index=13)
  - [A Machine-Readable File Format Specification for HDF5](https://github.com/HDFGroup/hdf-clinic/blob/main/2025-04-01/A%20Machine-Readable%20File%20Format%20Specification%20for%20HDF5.md)
  - Gaining momentum!

## HDFView

(If you haven't done so already...) Take the [HDFView survey](https://forms.office.com/r/anbMgVaXyF)!

The HUG25 HDF viewer roundtable provided a good overview of the relevant tools, but no real discussion developed, because we ran out of time. (Plenty of room for improvement!)

### The HDFView Anthem

I nominate one of my all-time favorite songs, [Bruce Springsteen's *Dancing in the Dark*](https://www.youtube.com/watch?v=129kuDCQtHs), as the anthem of HDFView.

### Key [Lyrics](https://lyrics.lyricfind.com/lyrics/bruce-springsteen-dancing-in-the-dark)

```
I check my look in the mirror
Wanna change my clothes, my hair, my face
Man, I ain't gettin' nowhere
I'm just livin' in a dump like this
There's somethin' happenin' somewhere
Baby, I just know that there is

You can't start a fire
You can't start a fire without a spark
```

Doesn't that aptly describe HDFView's current state?

### Next Steps

My current laundry list. Chime in!

1. Document the development set up & *Getting Started* docs.
2. Fix the installer.
3. Fix `TreeView` population.
4. Fix the dataset `GridView` population.
5. Fix copy/paste, drag & drop, etc.
6. Data import/export just works.
7. Fix HDF5 extension support, e.g., support HSDS and external filters.
8. Deprecate HDFView plugins(?)
9. `HDFView++ = HDFView + AI`
10. Facelift(?)

![HDFView++](./HDFViewAI.png)

**What's the spark?**