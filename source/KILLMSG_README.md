# Killmsg - Q2Admin-Python Game Data Interface

This module provides a clean interface for q2admin-python plugins to access live game data from aq2-tng without modifying core TNG source files.

## Overview

The `killmsg.c` and `killmsg.h` files in aq2-tng export game state functions that q2admin-python can call to get information about players, such as:

- Health, score, team
- Kills, deaths, accuracy
- Current weapon
- Player position
- IP address
- And more...

## Files

### aq2-tng/source/
- `killmsg.h` - Header with function declarations
- `killmsg.c` - Implementation of game data access functions
- `game.h` - Updated to include function pointers in `game_export_t`
- `g_main.c` - Updated to initialize the function pointers
- `Makefile` - Updated to compile killmsg.o

### q2admin-python/src/
- `q2a_python_export.c` - Python bindings for the killmsg functions
- `q2a_python.h` - Header declarations for Python exports

### q2admin-python/plugins/
- `playerstats.py` - Example plugin demonstrating usage

## Building

### Build aq2-tng with killmsg support:

```bash
cd aq2-tng/source
make clean
make
make install
```

This will compile killmsg.c and link it into the game DLL.

### Build q2admin-python:

```bash
cd q2admin-python
make clean
make
```

## Available Functions

All functions are available via the `gi` module in Python. Client numbers are 1-based in Python (0-based in C).

### Player Information
- `gi.GetPlayerHealth(client)` - Get health (-1 if invalid)
- `gi.GetPlayerScore(client)` - Get score/frags
- `gi.GetPlayerTeam(client)` - Get team number (0=none, 1-3=teams)
- `gi.GetPlayerKD(client)` - Get (kills, deaths) tuple
- `gi.GetPlayerName(client)` - Get player name
- `gi.GetPlayerIP(client)` - Get IP address
- `gi.GetPlayerWeapon(client)` - Get current weapon name
- `gi.IsPlayerAlive(client)` - Check if alive (returns bool)

### Player Stats
- `gi.GetPlayerAccuracy(client)` - Get accuracy percentage (0-100)
- `gi.GetPlayerHeadshots(client)` - Get headshot count
- `gi.GetPlayerStats(client)` - Get formatted stats string

### Player Position
- `gi.GetPlayerOrigin(client)` - Get (x, y, z) position tuple

### Server Information
- `gi.GetPlayerCount()` - Get number of active players
- `gi.GetMapName()` - Get current map name

## Usage Example

```python
import q2a_gi as gi

def ClientCommand(client: int):
    """Handle the 'stats' command"""
    cmd = gi.argv(0).lower()
    
    if cmd == "stats":
        name = gi.GetPlayerName(client)
        health = gi.GetPlayerHealth(client)
        kills, deaths = gi.GetPlayerKD(client)
        accuracy = gi.GetPlayerAccuracy(client)
        weapon = gi.GetPlayerWeapon(client)
        
        gi.cprintf(client, gi.PRINT_HIGH, f"\n--- Stats for {name} ---\n")
        gi.cprintf(client, gi.PRINT_HIGH, f"Health: {health}\n")
        gi.cprintf(client, gi.PRINT_HIGH, f"K/D: {kills}/{deaths}\n")
        gi.cprintf(client, gi.PRINT_HIGH, f"Accuracy: {accuracy:.1f}%\n")
        gi.cprintf(client, gi.PRINT_HIGH, f"Weapon: {weapon}\n")
        
        return True
    
    return False
```

## Plugin Example

See `q2admin-python/plugins/playerstats.py` for a complete example plugin that provides:

- `stats` - Show your stats
- `stats <player_num>` - Show another player's stats  
- `players` - List all players
- `playerinfo <num>` - Show detailed player info

To enable it, add to your `config.py`:

```python
plugins = {
    'playerstats': {},
    # ... other plugins
}
```

## Design Philosophy

The killmsg module keeps all game data access code in a single, separate file to:

1. **Avoid polluting core TNG code** - All q2admin-python interface code is isolated
2. **Easy to maintain** - One file to update when adding new functions
3. **Clean separation** - Game logic stays in TNG, plugin API stays in killmsg
4. **Backward compatible** - Older q2admin builds still work, just without these functions

## Adding New Functions

To add a new game data function:

1. Add declaration to `aq2-tng/source/killmsg.h`
2. Implement in `aq2-tng/source/killmsg.c`
3. Add function pointer to `game_export_t` in `aq2-tng/source/game.h`
4. Initialize the pointer in `aq2-tng/source/g_main.c` (GetGameAPI)
5. Add Python binding in `q2admin-python/src/q2a_python_export.c`
6. Add declaration in `q2admin-python/src/q2a_python.h`
7. Update API documentation

## Notes

- Client numbers in Python are 1-based (client 1 = first player)
- Client numbers in C are 0-based (client 0 = first player)
- The conversion is handled automatically by the Python bindings
- Functions return safe defaults if player not found (empty string, -1, 0, etc.)
- All functions check for valid client numbers and handle NULL pointers safely

## Compatibility

- Requires aq2-tng with killmsg.c compiled in
- Works with any Python 3.6+ version
- Functions gracefully return defaults if aq2-tng doesn't support them
- Backward compatible with older aq2-tng builds (functions just return empty/zero values)
