# HDF5 SSP Survey Update

We have a [standing survey](https://forms.gle/nD6hmRUd9h5H2KM19) that helps us assess the needs of all sectors using HDF5—from open science to industry and government. The survey takes about 10–15 minutes.
To date, we have received 33 responses, and I want to thank all who responded for their time and insights.

If you have not yet taken the [survey](https://forms.gle/nD6hmRUd9h5H2KM19), we still want to hear from you!

In today's clinic, I want to share a few observations.

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






## Acknowledgement

> This material is based upon work supported by the U.S. National Science Foundation under Federal Award No. 2534078. Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.​
