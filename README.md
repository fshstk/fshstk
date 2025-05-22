# Read Me

```text
              ██████          █████                              █████    █████
             ███░░███        ░░███                              ░░███    ░░███
            ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
           ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
          ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
            ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
            █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
          ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

         fantastic  spatial  holophonic               synthesis    tool    kit
```

![ci](https://github.com/fshstk/fshstk/actions/workflows/ci.yml/badge.svg)
[![sonarcloud](https://sonarcloud.io/api/project_badges/measure?project=fshstk_fshstk&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=fshstk_fshstk)
![ci](https://github.com/fshstk/fshstk/actions/workflows/sync-docs.yml/badge.svg)

- docs: [docs.fshstk.com](https://docs.fshstk.com)
- code: [github.com/fshstk/fshstk](https://github.com/fshstk/fshstk)

> [!IMPORTANT]
> _If you came here from my [poster presentation](https://pizza.fshstk.com/u/Ambisonium.pdf) at the
> AES convention (or from anywhere else for that matter) and you'd like to try out the Ambisonium
> synth yourself:_
>
> First of all, hi and welcome! 👋 I'm happy you're here. Please note that Ambisonium and the rest
> of the plugins that make up this repository are still very much in early development. This means
> that
>
> - there are no downloadable releases right now;
> - if you want to use it you will have to compile it yourself, which may be tricky if you don't
>   have at least some level of knowledge of CMake;
> - there may be bugs and unexpected behaviour, so be very cautious with volume;
> - future versions are not guaranteed to be backwards compatible in DAW projects.
>
> If you're still interested but don't know how to build the plugin yourself, I'm happy to help.
> Feel free to send me a mail; my address is in the PDF above.

## Motivation

This repository is an attempt at trying out a few creative ideas in Ambisonics/3D Audio, with a
focus on producing music directly in the HOA domain. There are some great existing tools out there,
but they are not geared specifically towards making music.

It also satisfies my itch to create something with the JUCE framework, but abstracting away all the
parts I don't like so much and replacing them with an API that is (imho) a little easier to use, and
a little harder to use incorrectly.

I'm also trying out writing extensive Doxygen documentation, so I hope it will be pretty simple to
read the docs and try out your own things. The architecture is pretty modular, things can be chained
together pretty easily.

Code readability and clean architecture are the top priorities. Even more so than efficiency, which
I guess sounds crazy for real-time audio code; but it's working well enough so far, and you get a
lot of maintainability (imho) in return for a little bit of efficiency. I believe in writing it the
way you want it to be written first, and then
[optimising later](https://wiki.c2.com/?PrematureOptimization) if
[necessary](https://wiki.c2.com/?ProfileBeforeOptimizing).

> This repository is inspired by (and some parts have been rewritten from) the free and open source
> [IEM Plugin Suite](https://plugins.iem.at).

## Download

> soon

## Building your own plugins

> also soon
