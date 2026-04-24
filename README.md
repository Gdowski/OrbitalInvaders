# 🪐 Orbital Invaders

A Space-Invaders-inspired arcade shooter where the player orbits Earth and defends it against waves of space invaders and asteroids.

Built with **Unreal Engine 5.7.4** in **C++** and **Blueprints** as a recruitment assignment.

---

## Gameplay

- The player pilots a ship orbiting Earth in a full 360° circle.
- Waves of **invaders** approach along concentric orbits, jumping closer each cycle. If any invader breaches the player's orbit, the game is over.
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



## Architecture notes

**Coupling in overlap handlers** - `Asteroid::HandleOverlap` (and similar) directly casts to concrete classes
(`AProjectile`, `AInvader`, etc.). If I had more time I'd replace it with `IDamageable` interface.

**Score table as a switch** - `AOrbitalGameState::AddScoreFor` uses `switch` for mapping score events to 
point values. If I had more time this would be a `UDataTable`.

---

## Clone and build

```bash
git clone <repo-url>
cd OrbitalInvaders
git lfs pull
```

---
    
    ## Code style — header layout
    
    To keep headers predictable, every `UCLASS` in this project follows this section order. Each access specifier appears **at most once** per class.
    
    ```cpp
    class ORBITALINVADERS_API AFoo : public AActor
    {
        GENERATED_BODY()
    
    public:
        // 1. Constructor
        AFoo();
    
        // 2. Public API (UFUNCTION BlueprintCallable / BlueprintPure)
        UFUNCTION(BlueprintCallable, Category = "Combat")
        int32 ApplyDamage(int32 Amount);
    
        // 3. Public delegates (UPROPERTY BlueprintAssignable)
        UPROPERTY(BlueprintAssignable)
        FOnDiedDelegate OnDied;
    
    protected:
        // 4. Virtual overrides
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
    
        // 5. Components (VisibleAnywhere - protected so subclasses can reach them)
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        TObjectPtr<USphereComponent> CollisionComponent;
    
        // 6. Config (EditAnywhere - tunable in BP)
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
        int32 MaxHealth = 3;
    
        // 7. SFX / VFX - kept separate from gameplay config for clarity
        UPROPERTY(EditAnywhere, Category = "FX")
        TObjectPtr<USoundBase> HitSound;
    
        // 8. Runtime state exposed for debugging (VisibleAnywhere)
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
        int32 CurrentHealth = 0;
    
        // 9. Virtual hooks for subclasses (protected, not private)
        virtual void OnDeath();
    
    private:
        // 10. Overlap / delegate callbacks
        UFUNCTION()
        void HandleOverlap(UPrimitiveComponent*, AActor*, UPrimitiveComponent*,
                           int32, bool, const FHitResult&);
    
        // 11. Internal helpers (private non-virtual methods)
        void DoInternalStuff();
    
        // 12. Pure runtime state (no UPROPERTY - timers, flags, counters)
        bool bIsCoolingDown = false;
        FTimerHandle CooldownTimer;
    };
```

