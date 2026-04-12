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
    - [x] Numerical stability (subtract max before exp)
- [x] Loss function (cross-entropy: -log(probability of target word))
- [x] Backpropagation / gradient computation
  - [x] Gradient of softmax + cross-entropy (predicted - actual)
  - [x] Gradient with respect to embedding weights
    - [x] Review: https://web.eecs.umich.edu/~justincj/teaching/eecs498/FA2020/linear-backprop.html
- [x] Weight update (SGD: weight = weight - learning_rate × gradient)
- [x] Training loop
  - [x] Build training pairs from text (context window → next word)
  - [x] Iterate over data, forward pass → loss → backward pass → update
- [x] Evaluate with cosine similarity (do similar words cluster?)
- [ ] Inference (text → embedding vector)
- [x] Save / load model weights
- [x] CLI interface (train, eval, similar)

## Build

```
make        # compile
make clean  # remove build artifacts
```

Requires `gcc` and `make`.

## Usage

```
# Train a model
./emb3d train <vocab> <data> <window_size> <emb_size> <lr> <epochs> <output_model>
./emb3d train data/vocab_atomic.txt data/train_20000.txt 3 64 0.01 1 output/model_e64_vAtomic.bin

# Evaluate with predefined word pairs
./emb3d eval <vocab> <model>
./emb3d eval data/vocab_atomic.txt output/model_e64_vAtomic.bin

# Check similarity between two words
./emb3d similar <vocab> <model> <word_a> <word_b>
./emb3d similar data/vocab_atomic.txt output/model_e64_vAtomic.bin software hardware
```

## Evaluation

The model is evaluated by measuring cosine similarity between word pairs that should be semantically similar (e.g. software/hardware, linux/kernel) and pairs that should be dissimilar (e.g. software/river, linux/ocean). Each group reports a mean and standard deviation.

To determine whether the model has learned meaningful distinctions, we compute a compatibility score:

```
sigma = sqrt(std_similar^2 + std_dissimilar^2)
compatibility = (avg_similar - avg_dissimilar) / sigma
```

This is the separation between the two group means measured in units of their combined uncertainty. A compatibility score above **2** means the separation is statistically significant — the model reliably places similar words closer together than dissimilar ones, beyond what random variation would explain.

## Results

Trained on 20,000 tech-filtered Wikipedia articles (~14.4M tokens), 25-word atomic vocab, embedding size 64, window size 3, 1 epoch. Loss: 0.018.

```bash
make && ./build/emb3d train data/vocab_atomic.txt data/train_20000.txt 3 64 0.01 1 output/model_e64_vAtomic.bin
```

### Similar Pairs
| Word A | Word B | Cosine Similarity |
|--------|--------|:-:|
| software | hardware | 0.9022 |
| algorithm | data | 0.7859 |
| network | internet | 0.8875 |
| server | database | 0.8502 |
| linux | kernel | 0.8701 |
| encryption | security | 0.7893 |
| processor | memory | 0.7756 |
| code | programming | 0.7671 |

### Dissimilar Pairs
| Word A | Word B | Cosine Similarity |
|--------|--------|:-:|
| software | river | 0.6696 |
| algorithm | painting | 0.6408 |
| network | forest | 0.6555 |
| server | mountain | 0.7464 |
| linux | ocean | 0.6862 |
| encryption | garden | 0.6329 |
| processor | novel | 0.7537 |
| code | weather | 0.6287 |

```
Similar pairs:    0.8287 ± 0.0514
Dissimilar pairs: 0.6769 ± 0.0459
Compatibility:    2.2047 (sep=0.1519, σ=0.0689)
```

At **2.2σ**, the model shows statistically significant separation between similar and dissimilar word pairs.

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c/#disclaimer
