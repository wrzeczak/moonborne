# A text-based political simulator.

---

Moonborne is a game in which, by controlling a character, you run for office and seek whatever political goals you may have. Boasting a complex political system beyond two-party systems commonly simulated, Moonborne will hopefully be relatively realistic to how real politics functions. This project is part of a larger project in which I seek to implement this system into a procedurally-generated map game a la HOI4 or Stellaris to have fully politically functioning countries battling it out and politicking all at the same time. It's for this reason that Moonborne is rather bare-bones visually, because the real important part is the underlying simulation.

### How to Play

When you first start the game, you'll be greeted by a traditional character creation screen, allowing you to input not only basic character info but also your political leanings on a 4-axis scale. You will then proceed, day by day, with building your career from nothing, running for office, and interacting with other (not real) people in your journey.

### Window Layout

```
######################################
#               HEADER               #
######################################
#                         #          #
#                         #          #
#                         #          #
#        INTERACT         #   INFO   #
#                         #          #
#                         #          #
#                         #          #
######################################
```

All player interaction, which is done mostly by typing, will be done through the `interact` window. Information, grabbed auto-magically by the game or by request of the player will be displayed in the `info` window. Game status messages will be displayed in the `header` window.

---

**Support Libraries** -- [TOML11](https://github.com/ToruNiina/toml11) by Dr. Toru Niina and [ncurses](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)

---

To build for yourself, install the support library/libraries as instructed on their pages, nothing else should be required; after installing, enter `src/` and `make run`, which will compile and run the executable. For more technical details on the structure of the codebase, see [this helpful file](https://github.com/wrzeczak/moonborne/blob/source/src/STRUCTURE.md).

Please note this program is currently Linux-only. I plan to make a Windows and potentially Mac (though I don't have a way to test on Mac currently) port later down the road.
