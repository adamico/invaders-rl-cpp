# Game Design Document: Volumetric Breach (Working Title)
**Concept:** A 2.5D Orthographic Isometric Evolution of the Fixed-Shooter Genre

---

## 1. Core Paradigm & Perspective
The game translates the fixed-screen mechanics of a traditional 2D arcade shooter into a true 3D coordinate space viewed through a **fixed orthographic isometric projection**. 

```
   [ Orthographic Viewport ]
       /                 \
      /   [3D Enemy Swarm] \
     /     Moving down Z'   \
    /                         \
   +---------------------------+  <-- [Player Plane (X', Y')]
```

* **The Orthographic Rule:** There is no perspective-based scaling or vanishing points. Objects maintain uniform dimensions regardless of their depth ($z'$). This creates a high-readability, surgical tactical arena where spatial relationships, gaps, and sizes remain perfectly constant and predictable.
* **Spatial Occupation:** Objects are deliberately large relative to the screen size. This shifts the gameplay loop away from pixel-perfect precision shooting and toward **spatial exclusion** and **positional intent**. The player negotiates volume, not pixels.

---

## 2. Spatial Mapping & Constraints

The game loop operates within a strict three-dimensional coordinate system mapped to distinct mechanical functions:

| Axis | System Mapping | Gameplay Function |
| :--- | :--- | :--- |
| **$x'$ Axis** | Horizontal | Lateral player movement and horizontal swarm pacing. |
| **$y'$ Axis** | Altitude | Vertical player movement and enemy layer positioning. |
| **$z'$ Axis** | Depth / Encroachment | The "Timer." The axis along which the enemy wall marches toward the player. |

* **The Player Boundary ("The Back Wall"):** The player is entirely constrained to a flat, vertical 2D plane at the very back of the map ($z'_{player}$). They can move freely across the horizontal ($x'$) and altitude ($y'$) axes, but cannot move through depth.
* **The Encroachment:** The enemy swarm spawns deep in the scene ($z'_{max}$) and advances step-by-step down the $z'$ axis toward the player. The game-over condition triggers when the physical volume of the swarm breaches the player's $z'$ plane.

---

## 3. The "Volumetric Breach" Gameplay Loop

By shifting the enemy formation from a flat 2D grid to a deep, multi-layered 3D block, the core objective evolves from simple attrition (kill everything) to **strategic penetration**. 

```
[Swarm Volume] ──> Moving toward Player
┌──────────────────┐
│  H  [S]  H   H   │   H = Hard Enemy (Shield)
│  H   |   H   H   │   [S] = Soft Spot (Weak Enemy)
│  H   V   H   H   │   | / V = Core Path of Least Resistance
└──────────────────┘
       |||||
[Player Plane (X', Y')]
```

### Tactical Geometry
The player views the deep enemy swarm as a series of channels, gates, and tunnels passing through their forward-facing $x'-y'$ viewport. Success relies on analyzing this 3D volume and choosing an engagement strategy:

* **The Piercing Objective:** Players do not need to clear the entire screen to progress. They can focus their fire on a single vertical or horizontal column, drilling a hole directly through the oncoming wall to create a safe zone where they can survive the wave's impact.
* **Risk vs. Efficiency (Soft Spots):** Formations are multiple layers deep. They feature localized "soft spots" composed of weaker, low-health enemies. However, these structural weaknesses are strategically surrounded or fronted by heavily armored "hard" enemies with larger hitboxes, forcing the player to decide whether to expend ammo/time eroding a shield or find an alternate path.
* **Bonus Attrition:** While piercing the wall allows survival and progression, completely wiping out a volumetric wave or clearing high-threat armor units awards massive score bonuses, satisfying high-skill players.

---

## 4. Key Design Advantages over 2D Clones

1. **Verticality as Defense:** Altitude ($y'$) is no longer a ticking doom clock; it is an active evasion and engagement plane. Players must rapidly dive or boost vertically to line up shots with specific enemy layers or slide into moving structural gaps.
2. **Absolute Visual Clarity:** Because size does not change with distance, collision zones are unambiguous. If a bullet or player aligns with a gap on the screen, it will clear that gap mathematically, eliminating the depth-perception errors common in standard perspective-based 3D shooters.
3. **Clustered Wave Progression:** Instead of uniform grids moving side-to-side, waves act as distinct, heavy architectural shapes moving through space, introducing puzzle-like layout designs to the shooter format.
