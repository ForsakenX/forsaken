# Forsaken

This repo holds the community port of Forsaken!

Check out the [Wiki](https://github.com/ForsakenX/forsaken/wiki) for more information.

## Dependencies

- [OpenGL](https://opengl.org)
- [OpenAL](https://www.openal.org)
- [SDL-1.2](https://github.com/libsdl-org/SDL-1.2)
- [SDL](https://github.com/libsdl-org/SDL)
- [lua5.4](https://www.lua.org/)
- [luasocket](https://github.com/lunarmodules/luasocket)
- [enet](https://github.com/lsalzman/enet/)

## Build

POSIX/MSYS2:
```sh
cd forsaken
make SDL=1 GL=3
```

## Run

Edit `forsaken.sh` and replace `/usr/lib64` with your `$PREFIX/$libdir/lua/5.4`

```sh
cd forsaken
./forsaken.sh
```

## Known bugs
- SDL2 controls not working
- Selecting last level "fishy" exits game
- no sound/music support implemented
