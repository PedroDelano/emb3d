from datasets import load_dataset
import re

dataset = load_dataset("wikimedia/wikipedia", "20231101.en", split="train", streaming=True)

NUM_ARTICLES = 100
output_path = "../data/train_small_100.txt"

with open(output_path, "w") as f:
    for i, article in enumerate(dataset):
        if i >= NUM_ARTICLES:
            break

        text = article["text"]
        text = re.sub(r'\n+', ' ', text)
        text = re.sub(r'\s+', ' ', text)
        text = text.strip()

        if len(text) > 100:
            f.write(text + "\n")

        if (i + 1) % 25 == 0:
            print(f"Processed {i + 1} articles")

print(f"Done. Saved to {output_path}")
