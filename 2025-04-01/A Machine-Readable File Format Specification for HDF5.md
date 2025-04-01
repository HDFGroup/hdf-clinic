# A Machine-Readable File Format Specification for HDF5

> People's language reflects reality; LLM's language reflects people. (Prompt Engineering for LLMs, O'Reilly)

## What

It's a formal description of a file format that is structured in a way that software tools (i.e. machines) can parse, analyze, and often use directly—for example, to read, write, validate, or generate files that conform to that format.

This contrasts with human-readable specs (like prose in a PDF or web page), which require manual interpretation by a developer.

Key Features of Machine-Readable Specifications:
  
  - **Structured syntax:** Usually written in a data serialization language (e.g., JSON, XML, YAML) or a domain-specific language (DSL).
  - **Unambiguous:** Defines the file format with precision—fields, sizes, types, byte order, constraints, and sometimes relationships between fields.
  - **Tool-friendly:** Can be used to automatically generate parsers, serializers, documentation, and test cases.

## Why

- **Trust:** Owners/maintainers don't play footsie with the spec.
- **Portability:** Makes format specs portable across languages, systems, and teams.
- **Maintainability:** (Ideally) Changes to the spec propagate automatically to tooling.
- **Validation & Debugging:** It is Easier to verify whether a file conforms to the spec and whether an implementation makes assumptions beyond the spec.
- **Reverse Engineering:** Helps document legacy or proprietary formats.

## Simple, Complicated, and Complex Problems

What kind of problem are we dealing with?

**Source:** [Complicated and Complex Systems: What Would Successful Reform of Medicare Look Like?](https://publications.gc.ca/collections/Collection/CP32-79-8-2002E.pdf)

| Following a Recipe | Sending a Rocket to the Moon | Raising a Child |
| :---         |     :---:      |          ---: |
| The recipe is essential   | Formulae are critical and necessary     | Formulae have a limited application    |
| Recipes are tested to assure easy replication     | Sending one rocket increases assurance that the next will be OK       | Raising one child provides experience but no assurance of success with the next      |
| No particular expertise is required. But cooking expertise increases success rate     | High levels of expertise in a variety of fields are necessary for success       | Expertise can contribute but is neither necessary nor sufficient to assure success      |
| Recipes produce standardized products     | Rockets are similar in critical ways       | Every child is unique and must be understood as an individual     |
| The best recipes give good results every time     | There is a high degree of certainty of outcome       | Uncertainty of outcome remains     |
| Optimistic approach to problem possible     | Optimistic approach to problem possible       | Optimistic approach to problem possible     |

This looks like a problem that has traits of a complicated problem, but that can be broken up into a set of simple steps.

## How

- Common Lisp: Read chapters 24 and 25 in [Practical Common Lisp](https://gigamonkeys.com/book/) by Peter Seibel
- YAML fans enjoy [Kaitai](https://kaitai.io/)
- I like [GNU poke](https://www.jemarch.net/poke)
- If you just want an independent implementation, consider [PureHDF](https://apollo3zehn.github.io/PureHDF/)

So far, nothing new here. (I talked about this a few years ago, but I won't have the time to "sweat the details.")

How about asking [ChatGPT](https://chatgpt.com/) for a helping hand? Worst case: Come back another time...

## Dramatis Personae

- [HDF5 File Format Specification Version 2.0](https://support.hdfgroup.org/documentation/hdf5/latest/_f_m_t2.html) (no introduction needed)
- [GNU poke 4.3](https://www.jemarch.net/poke-4.3-manual/poke.html) (I recommend you watch José Marchesi's [Introduction to GNU poke](https://www.youtube.com/watch?v=KZ8meNZ_IhY).)
- [ChatGPT GPT-4o](https://openai.com/index/gpt-4o-system-card/) (no introduction needed)

## A Recent Conversation With ChatGPT

Doing live demos with ChatGPT is hard for several reasons:
- LLMs are inherently probabilistic. Even with the same prompt, they might give slightly different answers each time. In a live demo, this can backfire if you're expecting a specific outcome or format.
- ChatGPT performs best when it has the right context. In a live demo, if the setup doesn’t fully establish what the model “knows” or if earlier context is missing or ambiguous, the results can be off.
- What seems clear to a person might not be clear to the model. If your prompt is slightly off or vague, it may interpret it in an unintended way—and there's no room for trial and error in a live setting.
- While not always the model’s fault, any delay in response due to internet hiccups or API timeouts can kill the flow of a demo.
- If the demo relies on plugins, APIs, or tool use (like file uploads, code execution, or web browsing), anything breaking in that chain can derail the demo—especially since these components don’t always behave identically between sessions.
- Demo stress is real. You might mistype something, forget a step, or rush. Unlike pre-recorded content, there’s no second take.
- If the demo was built and tested on a specific model version (e.g., GPT-4-turbo), and that version changes or is updated just before the demo, behavior might subtly shift.

*I will do better next time by doing the following:*
- Use an intro prompt: always start with a clear system or setup prompt (even hidden if you're using the API or an app).
- Use pre-filled prompts or buttons instead of typing them live.
- Start with a simple prompt or two to load context and prime the model's behavior.
- Use `temperature = 0` for consistency
- Build the demo with Custom GPTs or Pre-loaded Context (ChatGPT Pro)

With these caveats, here is a [link](https://chatgpt.com/share/67ebf358-eb30-8000-a626-4e0bda18535e) to a recent conversation.

### Observations

- The code snippets are riddled with errors
- The logic/structure is sound, but have the `poke` manual handy if you want to try the suggestions
- I have not tried this, but a Custom GPT might help this (I'll report back next time)
- Maybe a more code-centric model (`o3-mini-high`) would yield better results?
- I have not tried Kaitai, which might yield better results

## Next Steps

Let's build a few useful tools together! Interested? Email [me](mailto:gheber@hdfgroup.org)!

### Use Cases

| Read-Only  | Read/Write |
| ------------- | ------------- |
| A better`h5debug` (`h5poke`?)  | Simple repair jobs  (e.g., `h5clear`, superblock fixes) |
| File consitency checker  | (Once we've mastered simple) Complicated repair jobs  |
| Reporting/visualization app. | Data transformer |
| Data extractor| ??? |
