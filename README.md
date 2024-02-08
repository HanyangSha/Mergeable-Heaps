# Mergeable-Heaps

A while back I decided to study the interesting topic of mergeable heaps. We know that ordinary binary heaps support merging in O(n), but it is possible to do much better. Binomial heaps (in the strict sense) allows O(lg n) merging, and Fibonacci heaps allows O(1). The key idea for these two types of mergeable heaps is to maintain a root list of multiple roots instead of just a single root. 

The most natural (at least in my opinion) order to understand Fibonacci heaps (which ultimately has the most optimal complexities) is binary heap -> binomial heap -> lazy binomial heap -> Fibonacci heap. The intuition is most clearly explained [in this post](https://stackoverflow.com/questions/19508526/what-is-the-intuition-behind-the-fibonacci-heap-data-structure). 

The repo gives my implementations of the relevant heaps in C++. Small note of caution: #include <bits/stdc++.h> may not work on your machine, in which case it is easy to just replace it with whatever libraries you need. I left some small comments in the code, some as explanations and some as remarks from the programmer's perspective. 

Remark: I was originally motivated to study this topic due to operations in Dijkstra's Algorithm. However, I've also seen others' claims that due to the large hidden constant in Fibonacci Heap, it may be faster to just use a simplier data structure, so take it with a grain of salt. 
