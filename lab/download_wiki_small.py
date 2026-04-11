from datasets import load_dataset
import re
import sys

num_articles = int(sys.argv[1]) if len(sys.argv) > 1 else 100
output_path = sys.argv[2] if len(sys.argv) > 2 else "../data/train_small_100.txt"

dataset = load_dataset("wikimedia/wikipedia", "20231101.en", split="train", streaming=True)

with open(output_path, "w") as f:
    for i, article in enumerate(dataset):
        if i >= num_articles:
            break

        text = article["text"]
        text = re.sub(r'\n+', ' ', text)
        text = re.sub(r'\s+', ' ', text)
        text = text.strip()

        if len(text) > 100:
            f.write(text + "\n")

        if (i + 1) % 25 == 0:
            print(f"Processed {i + 1} articles")

print(f"Done. {num_articles} articles saved to {output_path}")
