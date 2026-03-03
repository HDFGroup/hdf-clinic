# HDF5 SSP Survey Update

We have a [standing survey](https://forms.gle/nD6hmRUd9h5H2KM19) that helps us assess the needs of all sectors using HDF5—from open science to industry and government. The survey takes about 10–15 minutes.
I want to start by thanking all who responded for their time and insights.

If you have not yet taken the [survey](https://forms.gle/nD6hmRUd9h5H2KM19), we still want to hear from you!

In today's clinic, I want to share a few observations.

## Summary

**Respondents:** 33 total survey responses. Most questions had 26-31 answers (some optional fields). 

- Highest-priority threat areas: supply chain for libraries/bindings (14/31 = 45% rated Major/Critical) and malformed/poisoned files (12/31 = 39% Major/Critical). 
- Third-party plugins are common: 17/33 (52%) load third-party plugins at least occasionally; 7/33 (21%) report VOL/VFD/transforms usage (vs compression filters alone). 
- Offline constraints matter: 18/28 (64%) require solutions that work in offline/air-gapped environments. 
- Integrity/resilience gap: 10/33 (30%) experienced corruption/unreadability after crash/power loss in the last 12 months; average confidence that corruption would be detected is 2.86/5 (n=28). 
- Strong appetite for built-in integrity verification: 17/31 (55%) would adopt immediately; most others are conditional on overhead. 
- Performance budget is modest: 26/33 (79%) would accept up to 10% overhead for security features. 

## Key takeaways

1. For 42\% of respondents, supply chain security (SBOM, transitive deps.) is a major or critical concern
2. For 36\% of respondents, malformed/poisoned files (parser bugs) are a major or critical concern
3. 52\% regularly/occasionally load 3rd‑party plugins
4. 30\% saw corruption/unreadability after a crash or power loss.

**What this suggests:**

- Prioritize *supply-chain visibility* (SBOM/CVE communication) and *defensive parsing* for untrusted files.​
- Build a “safe open” posture: read‑only mode + resource limits as defaults for untrusted inputs.​
- Plan for *offline/air‑gapped* verification: 18 respondents selected this requirement (of 28 who answered).​
- Strong appetite for built‑in integrity checks: 16 want metadata+data; 14 are conditional.​​

## Who responded - roles, org. types, and geography (self‑reported)

### Organization type

- Private company: 12 (36%) 
- Government/National lab: 9 (27%) 
- University: 7 (21%) 
- Non-profit: 3 (9%) 
- Independent consultant: 1 (3%)
- Startup: 1 (3%) 

### Where HDF5 is used (multi-select) 

- Desktop/workstation: 27 
- On-prem filesystems: 14 
- HPC (MPI/parallel I/O): 14 
- Cloud/object storage (HSDS/S3): 7 
- Edge/embedded: 4 


### Sensitive data frequency

- Never: 12 (36%) 
- Sometimes: 9 (27%) 
- Rarely: 7 (21%) 
- Often: 4 (12%) 
- Always: 1 (3%) 

## Threat prioritization

![Threat concerns](./Threat%20concerns%20rated%20major-critical.png)

## Top concerns for the next 12 months

![Top concerns](./Top%20concerns%20for%20next%2012%20months.png)

## Plugin use

![Plugin use](./Third-party%20plugin%20loading%20frequency.png)

## Environment constraints (offline, policy, and deployment)

![Environment constraints](./Signature%20verification%20environment%20constraints.png)

## Integrity & resilience (corruption detection and recovery) 

![Integrity & resilience](./Crash-power%20loss%20related%20HDF5%20corruption%20in%20past%2012%20months.png)

![Corruption detection confidence](./Confidence%20corruption%20would%20be%20detected.png)

## Adoption and performance tradeoffs

![Integrity verification adoption willingness](./Willingness%20to%20adopt%20built-in%20integrity%20verification.png)

![Acceptable overhead](./Acceptable%20performance%20overhead%20for%20security%20features.png)



## Preliminary implications for HDF5 SSP SIG priorities 

- Prioritize: (a) supply-chain guidance and tooling (SBOMs, advisories, packaging patterns), and (b) hardening against malformed files (fuzzing, conformance corpora, safe parsing defaults). 
- Design signing/verification for offline-first deployments (air-gapped HPC), with optional online transparency logs where available. 
- Define a “safe mode” profile for untrusted inputs: object/attribute caps, link-depth limits, chunk limits, bounded metadata cache, and open-time timeouts. 
- Pair technical mitigations with operational assets: best-practice deployment guides, disclosure/patch cadence guidance, and compliance tests (e.g., signed-plugin checks). 
- Treat integrity detection as a product feature: make it easy to turn on, easy to measure overhead, and easy to integrate into CI/CD pipelines. 

## Acknowledgement

> This material is based upon work supported by the U.S. National Science Foundation under Federal Award No. 2534078. Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.​
