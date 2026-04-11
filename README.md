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
./emb3d train <vocab> <data> <window_size> <emb_size> <output_model>
./emb3d train data/vocab_small.txt data/train_small.txt 3 32 data/model.bin

# Evaluate with predefined word pairs
./emb3d eval <vocab> <model>
./emb3d eval data/vocab_small.txt data/model.bin

# Check similarity between two words
./emb3d similar <vocab> <model> <word_a> <word_b>
./emb3d similar data/vocab_small.txt data/model.bin machine learning
```

## Results (small model)

Trained on 3 Wikipedia articles (~810 tokens), 404-word vocab, embedding size 32, window size 3, 3 epochs. Loss: 5.94 → 0.002.

### Similar Pairs
| Word A | Word B | Cosine Similarity |
|--------|--------|:-:|
| machine | learning | 0.8710 |
| sentiment | analysis | 0.8198 |
| supervised | unsupervised | 0.8164 |
| statistical | methods | 0.7109 |
| data | datasets | 0.7025 |
| text | language | 0.6966 |
| neural | network | 0.6883 |
| good | excellent | 0.6307 |

### Dissimilar Pairs
| Word A | Word B | Cosine Similarity |
|--------|--------|:-:|
| hotel | statistical | 0.7832 |
| machine | food | 0.7505 |
| anger | data | 0.7348 |
| hotel | neural | 0.7344 |
| disgust | optimization | 0.7269 |
| food | parsing | 0.7153 |
| praise | computing | 0.6864 |
| anger | algorithms | 0.6481 |

Similar pairs average **0.73**, dissimilar pairs average **0.72**. The gap is narrow due to the tiny training set — with more data the separation should improve.

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c/#disclaimer
