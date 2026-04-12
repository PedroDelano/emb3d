from datasets import load_dataset
import random
import re
import sys

random.seed(42)

sizes = [5, 10, 20, 50, 100, 1000, 5000, 10000, 20000]

# Strong signals: one match in title or text is enough
STRONG_KEYWORDS = [
    "software",
    "programming",
    "algorithm",
    "database",
    "internet",
    "processor",
    "semiconductor",
    "artificial intelligence",
    "machine learning",
    "operating system",
    "encryption",
    "cybersecurity",
    "blockchain",
    "cloud computing",
    "data structure",
    "compiler",
    "open source",
    "neural network",
    "deep learning",
    "natural language processing",
    "virtualization",
    "firmware",
    "microchip",
    "transistor",
    "computer science",
    "source code",
    "web browser",
    "search engine",
    "computer network",
    "programming language",
    "video game",
    "silicon valley",
    "supercomputer",
    "mainframe",
]

# Title-only keywords: too ambiguous in body text, but specific enough in titles
TITLE_KEYWORDS = [
    "computer",
    "computing",
    "linux",
    "javascript",
    "html",
    "cpu",
    "gpu",
    "ethernet",
    "wifi",
    "bluetooth",
    "usb",
    "tcp",
    "http",
    "google",
    "microsoft",
    "intel",
    "nvidia",
    "ibm",
    "amd",
    "github",
    "stackoverflow",
    "wikipedia",
    "android",
    "ios",
    "robotics",
    "telecommunications",
    "electronics",
]


def is_tech_article(title, text):
    title_lower = title.lower()
    text_lower = text[:1000].lower()
    for kw in STRONG_KEYWORDS:
        if kw in title_lower or kw in text_lower:
            return True
    for kw in TITLE_KEYWORDS:
        if kw in title_lower:
            return True
    return False


dataset = load_dataset(
    "wikimedia/wikipedia", "20231101.en", split="train", streaming=True
)

# Stream through articles, keeping only tech-related ones
SCAN_LIMIT = 500000
pool = []

for i, article in enumerate(dataset):
    if i >= SCAN_LIMIT:
        break
    if len(pool) >= max(sizes):
        break

    title = article["title"]
    text = article["text"]
    text = re.sub(r"\n+", " ", text)
    text = re.sub(r"\s+", " ", text)
    text = text.strip()

    if len(text) > 100 and is_tech_article(title, text):
        pool.append(text)

    if (i + 1) % 5000 == 0:
        print(f"Scanned {i + 1} articles, found {len(pool)} tech articles")

print(f"Pool: {len(pool)} tech articles (scanned {i + 1} total)")

random.shuffle(pool)

for size in sizes:
    subset = pool[:size]

    # Write training data
    data_path = f"../data/train_{size}.txt"
    with open(data_path, "w") as f:
        for text in subset:
            f.write(text + "\n")

    print(f"  train_{size}.txt ({len(subset)} articles)")

print("Done.")
