import re
import sys

if len(sys.argv) < 3:
    print("Usage: python build_vocab.py <input_data> <output_vocab>")
    sys.exit(1)

input_path = sys.argv[1]
output_path = sys.argv[2]

words = set()
with open(input_path, "r") as f:
    for line in f:
        line = line.lower()
        line = re.sub(r'[!@#$%&*()\.,\?;:\"\'\-\[\]/\\]', ' ', line)
        for word in line.split():
            word = word.strip()
            if word:
                words.add(word)

vocab = sorted(words)
with open(output_path, "w") as f:
    for word in vocab:
        f.write(word + "\n")

print(f"Done. {len(vocab)} words saved to {output_path}")
