# emb3d

Embeddings model from scratch in C.

## TODO

- [x] Matrix struct and basic constructors (new, square, identity, free, print)
- [x] Matrix operations (add, subtract, scalar multiply, transpose)
- [x] Matrix multiplication
- [x] Dot product
- [ ] Tokenizer (map text to integer IDs)
  - [x] Load vocab file and count tokens
  - [ ] Build token-to-ID map (string → int)
  - [ ] Encode function (string → int ID)
  - [ ] Decode function (int ID → string)
  - [ ] Tokenize raw text (split + encode)
- [ ] Vocabulary builder
- [ ] Embedding lookup table (matrix of token vectors)
- [ ] Forward pass (token → embedding vector)
- [ ] Loss function (e.g. cosine similarity, negative sampling)
- [ ] Backpropagation / gradient computation
- [ ] Weight update (SGD or similar)
- [ ] Training loop
- [ ] Save / load model weights
- [ ] Inference (text → embedding vector)

## Build

```
make        # compile
make run    # compile and run
make clean  # remove build artifacts
```

Requires `gcc` and `make`.

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c/#disclaimer
