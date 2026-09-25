<div align="center">

# SUNSET CRUISE

**Los Angeles to San Francisco up the coast highway, racing the sun. A 16-bit arcade driving game on a console that never existed.**

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![SDL2](https://img.shields.io/badge/SDL-2-1e6fb8)
![License: MIT](https://img.shields.io/badge/license-MIT-green)

🎮 **[Play it in your browser](https://macncrash.github.io/s3cruise/)**

</div>

Four legs of the Pacific Coast Highway, from late afternoon to sunset:

- **Santa Monica:** from the pier to Malibu, with palms, sand blowing across the road, and the sun low over the bay.
- **Big Sur:** the Bixby cliffs, with the Pacific a long way down on your left.
- **Monterey:** cypress trees and the sea fog rolling in.
- **Golden Gate:** into the city as the sun goes down, the bridge towers against the sky.

Beat the checkpoint clock and pass the traffic of fifteen other cruisers. The
game has two cars, automatic or manual gears, a navigator calling the bends, the
car radio (88.1 THE BLADE, 101.5 NEON FM, 94.7 KOOL, 96.6 ARENA FM, and YOUR
MUSIC), gamepad support, and head-to-head races for up to four players on a LAN
or over the internet by IP address.

SUNSET CRUISE is a fork of S3 RUN, the arcade racer from
[(3) RALLY](https://github.com/macncrash/s3rally)'s multi-cart, rebuilt around
the coast. It runs on the same S3-16 console code, and every graphic, song
and sound is generated when it boots.

## Build and play

```bash
brew install sdl2        # or: sudo apt install libsdl2-dev
make
./s3
```

Arrows steer, C or Up accelerates, X or Down brakes, Q/W shift (manual), Tab
changes the radio station, Enter starts or pauses, and Esc goes back.
`./s3 --sim` drives every leg with the autopilot and reports.
`./s3 --versus-test` races consoles against each other over loopback UDP.
`make web` builds the browser version (needs Emscripten).

## License

MIT © macncrash
