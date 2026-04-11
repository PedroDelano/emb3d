from datasets import load_dataset
import re
import sys

sizes = [5, 10, 20, 50, 100, 1000, 5000]

dataset = load_dataset("wikimedia/wikipedia", "20231101.en", split="train", streaming=True)

# Collect all articles up to the max size
max_size = max(sizes)
articles = []

for i, article in enumerate(dataset):
    if i >= max_size:
        break

    text = article["text"]
    text = re.sub(r'\n+', ' ', text)
    text = re.sub(r'\s+', ' ', text)
    text = text.strip()

    if len(text) > 100:
        articles.append(text)

    if (i + 1) % 500 == 0:
        print(f"Downloaded {i + 1} articles")

print(f"Downloaded {len(articles)} articles total")

for size in sizes:
    subset = articles[:size]

    # Write training data
    data_path = f"../data/train_{size}.txt"
    with open(data_path, "w") as f:
        for text in subset:
            f.write(text + "\n")

    # Build vocab
    words = set()
    for text in subset:
        text = text.lower()
        text = re.sub(r'[!@#$%&*()\.,\?;:\"\'\-\[\]/\\]', ' ', text)
        for word in text.split():
            word = word.strip()
            if word and word.isascii() and word.isalpha():
                words.add(word)

    vocab_path = f"../data/vocab_{size}.txt"
    vocab = ["[UNK]"] + sorted(words)
    with open(vocab_path, "w") as f:
        for word in vocab:
            f.write(word + "\n")

    print(f"  train_{size}.txt ({len(subset)} articles) / vocab_{size}.txt ({len(vocab)} words)")

print("Done.")
