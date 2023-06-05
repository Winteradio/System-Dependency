# System Manager - README

This code demonstrates the System Manager, which manages systems in an Entity-Component-System (ECS) architecture. The code handles the dependencies between systems and provides a sorted execution order based on those dependencies.

## Purpose

The System Manager was created to simplify the handling of dependencies between systems in an ECS system. By automatically sorting the execution order based on dependencies, it eliminates the need for manually adjusting system indices when adding or removing systems.

## Algorithm Overview

1. The System Manager maintains a `Data` map to store system instances, a `Need` map to store system dependencies, and `Sequence` and `Done` queues for managing the execution order.
2. When a system is created using `Create()`, it is added to the `Data` map along with an empty dependency list in the `Need` map.
3. Dependencies are set using `SetDependency()`, where the dependent system's name is added to the dependency list of the main system.
4. To determine the execution order, the `Sort()` function is called. It uses a topological sorting algorithm.
5. The `Sort()` function initializes a `DepCount` map to keep track of the number of dependencies for each system.
6. It then iterates through the `Need` map and increments the dependency count for each system based on its dependencies.
7. Systems with no dependencies (count = 0) are pushed into the `Sequence` queue and marked as processed by decrementing their dependency count.
8. The function continues this process until all systems are processed and added to the `Sequence` queue.
9. Finally, the `Frame()` function executes the systems in the sorted `Sequence` queue, and the executed systems are moved to the `Done` queue.
10. The `Frame()` function repeats this process until all systems have been executed.

## Usage

1. Create instances of systems using the `Create()` function in the `SystemManager` class.
2. Set dependencies between systems using the `SetDependency()` function.
3. Call `Sort()` to sort the systems based on their dependencies.
4. Execute the systems in the sorted order using the `Frame()` function.
5. Clean up the systems using `Destroy()`.

Please refer to the code comments and the main function in the code for a usage example.