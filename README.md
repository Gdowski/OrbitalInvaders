# 🪐 Orbital Invaders

A Space Invaders–inspired arcade shooter where the player orbits Earth and defends it against waves of space invaders and asteroids.

Built with **Unreal Engine 5.7** in **C++** and **Blueprints** as a recruitment assignment.

---

## Gameplay

- The player pilots a ship orbiting Earth in a full 360° circle.
- Waves of **invaders** approach along concentric orbits, jumping closer each cycle — if any invader breaches the player's orbit, the game is over.
- The player fires projectiles radially outwards toward enemies; invaders fire inwards the same way.
- **Asteroids** cross the playfield with wrap-around, splitting into smaller pieces when shot.
- A rare **Special Invader** spirals in from the outer rim toward Earth.
- **Bunkers** positioned between the player and enemy orbits absorb incoming fire but degrade over time.
- Earth has its own health bar; a direct asteroid impact ends the game instantly.

### Endless Mode

The only game mode. Each wave increases enemy count, speed, fire rate, and asteroid frequency. The goal is to achieve the highest score possible.

---

## Controls

The ship moves along its orbit based on the direction the player pushes toward. Hold a direction to keep moving; release to stop.

| Input | Action |
|-------|--------|
| `A` / `D` | Move along orbit (directional) |
| `←` / `→` | Move along orbit (directional) |
| `Space`   | Fire |
| `Esc` | Pause |



## Clone and build

```bash
git clone <repo-url>
cd OrbitalInvaders
git lfs pull
```
