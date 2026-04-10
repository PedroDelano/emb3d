# emb3d

Embeddings model from scratch in C.

## TODO

- [x] Matrix struct and basic constructors (new, square, identity, free, print)
- [x] Matrix operations (add, subtract, scalar multiply, transpose)
- [x] Matrix multiplication
- [x] Dot product
- [x] Tokenizer (map text to integer IDs)
  - [x] Load vocab file and count tokens
  - [x] Constructing a linked list
  - [x] Build hash map (string → int)
  - [x] Build token-to-ID map (string → int)
  - [x] Encode function (string → int ID)
  - [x] Decode function (int ID → string)
  - [x] Tokenize raw text (split + encode)
- [x] Vocabulary builder
- [x] Embedding lookup table (matrix of token vectors)
- [x] Embedding lookup (token → embedding vector)
- [x] Forward pass (context words → next-word prediction)
  - [x] Combine input token embeddings into a single context vector
  - [x] Multiply embedding matrix by context vector → scores (vocab_size × 1)
  - [x] Softmax over scores → probability distribution
    - [ ] Numerical stability (subtract max before exp)
- [x] Loss function (cross-entropy: -log(probability of target word))
- [ ] Backpropagation / gradient computation
  - [ ] Gradient of softmax + cross-entropy (predicted - actual)
  - [ ] Gradient with respect to embedding weights
    - [ ] Review: https://web.eecs.umich.edu/~justincj/teaching/eecs498/FA2020/linear-backprop.html
- [ ] Weight update (SGD: weight = weight - learning_rate × gradient)
- [ ] Training loop
  - [ ] Build training pairs from text (context window → next word)
  - [ ] Iterate over data, forward pass → loss → backward pass → update
- [ ] Save / load model weights
- [ ] Inference (text → embedding vector)
- [ ] Evaluate with cosine similarity (do similar words cluster?)

## Build

```
make        # compile
make run    # compile and run
make clean  # remove build artifacts
```

Requires `gcc` and `make`.

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c/#disclaimer
