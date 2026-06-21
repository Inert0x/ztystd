# ZTY Standard Template Notes

This repository uses class-based templates under namespace `zty`.

## General Rules

- Headers are intended to be included directly in competitive-programming style code.
- Most modules are implemented as `class` with public members and methods for easy extension.
- Debug output is guarded by `#ifdef ZTY_DEBUG`.
- Define `ZTY_DEBUG_STDOUT` together with `ZTY_DEBUG` if debug output should go to `cout` instead of `cerr`.

## Extending Templates

Some templates only provide basic behavior. They are meant to be inherited or copied and modified for a specific problem.

Example:

```cpp
class MySegTree : public zty::segtree
{
public:
    using zty::segtree::segtree;

    // Override or add problem-specific merge, tag, query, or update logic here.
};
```

## Important Notes

- `DataStructure/SegmentTree.hpp` currently implements basic range-add/range-query style behavior. For max, min, gcd, assignment tags, persistent segment trees, or custom lazy propagation, inherit from it or modify the merge/tag logic.
- `DataStructure/Treap.hpp` implements an ordered multiset Treap with rank, kth, predecessor, and successor. It is not a sequence Treap by default.
- `Graph/Graph.hpp` only defines graph storage, traversal, and node information. Shortest path algorithms are in `Graph/ShortestPath.hpp`.
- `Graph/GraphTools.hpp` contains additional graph algorithms such as cycle detection, SCC, condensation graph, bridges/articulation points, Kruskal, and Dinic.
- `String/StringMatching.hpp` contains KMP and AC automaton. The AC automaton depends on `String/Trie.hpp`.
- `Matrix.hpp` is kept at the root because it is a math/linear-algebra module, not a data-structure module.

## Debug Compatibility

To make a new class compatible with `Debugger`, add:

```cpp
#ifdef ZTY_DEBUG
void debug(ostream& os) const
{
    os << "ClassName(field=" << field << ")\n";
}
#endif
```

Then use:

```cpp
zty::Debugger dbg;
dbg.log("object_name", object);
```

## Further Updates

Well, updates depend on what I've learnt.
