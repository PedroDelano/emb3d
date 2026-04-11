from datasets import load_dataset
import re

# Load English Wikipedia, streaming to avoid downloading everything
dataset = load_dataset("wikimedia/wikipedia", "20231101.en", split="train", streaming=True)

NUM_ARTICLES = 5000
output_path = "../data/train.txt"

with open(output_path, "w") as f:
    for i, article in enumerate(dataset):
        if i >= NUM_ARTICLES:
            break

        text = article["text"]

        # Basic cleanup: remove extra whitespace, keep only paragraphs
        text = re.sub(r'\n+', ' ', text)
        text = re.sub(r'\s+', ' ', text)
        text = text.strip()

        if len(text) > 100:  # skip very short articles
            f.write(text + "\n")

        if (i + 1) % 500 == 0:
            print(f"Processed {i + 1} articles")

print(f"Done. Saved to {output_path}")
