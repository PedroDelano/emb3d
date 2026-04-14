from datasets import load_dataset
from concurrent.futures import ThreadPoolExecutor, as_completed
import random
import re

random.seed(42)

sizes = [5, 10, 20, 50, 100, 1000, 5000, 10000, 20000]

# Strong signals: one match in title or text is enough
STRONG_KEYWORDS = [
    "linux",
    "software",
    "hardware",
    "algorithm",
    "server",
    "encryption",
    "blockchain",
]


def process_article(article):
    """Process a single article: clean text and check if it's tech-related.
    Returns cleaned text if tech article, None otherwise."""
    title = article["title"]
    text = article["text"]
    text = re.sub(r"\n+", " ", text)
    text = re.sub(r"\s+", " ", text)
    text = text.strip()

    if len(text) <= 100:
        return None

    title_lower = title.lower()
    text_lower = text[:1000].lower()
    if "disambiguation" in title:
        return None
    for kw in STRONG_KEYWORDS:
        if kw in title_lower or kw in text_lower:
            print(f"     [TECH ARTICLE FOUND]: {title}")
            return text
    return None


def write_subset(size, pool):
    """Write a training data file for a given size."""
    subset = pool[:size]
    data_path = f"../data/train_{size}.txt"
    with open(data_path, "w") as f:
        for text in subset:
            f.write(text + "\n")
    print(f"  train_{size}.txt ({len(subset)} articles)")


dataset = load_dataset(
    "wikimedia/wikipedia", "20231101.en", split="train", streaming=True
)

# Stream through articles, processing in thread pool
SCAN_LIMIT = 5000000
BATCH_SIZE = 512
NUM_WORKERS = 8

pool = []
scanned = 0
batch = []

for i, article in enumerate(dataset):
    if i >= SCAN_LIMIT:
        break
    if len(pool) >= max(sizes):
        break

    batch.append(article)
    scanned = i + 1

    if len(batch) >= BATCH_SIZE:
        with ThreadPoolExecutor(max_workers=NUM_WORKERS) as executor:
            results = executor.map(process_article, batch)
            for result in results:
                if result is not None:
                    pool.append(result)
                    if len(pool) >= max(sizes):
                        break
        batch = []

        if scanned % 5000 < BATCH_SIZE:
            print(f"Scanned {scanned} articles, found {len(pool)} tech articles")

# Process remaining batch
if batch and len(pool) < max(sizes):
    with ThreadPoolExecutor(max_workers=NUM_WORKERS) as executor:
        results = executor.map(process_article, batch)
        for result in results:
            if result is not None:
                pool.append(result)

print(f"Pool: {len(pool)} tech articles (scanned {scanned} total)")

random.shuffle(pool)

# Write all output files in parallel
with ThreadPoolExecutor(max_workers=len(sizes)) as executor:
    futures = [executor.submit(write_subset, size, pool) for size in sizes]
    for future in as_completed(futures):
        future.result()

print("Done.")
