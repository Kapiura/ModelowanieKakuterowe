import matplotlib.pyplot as plt
import random
import os

def init():
    monopolDisct = {}
    for i in range(40):
        monopolDisct[i] = 0
    return monopolDisct

def diceRoll():
    roll1 = random.randint(1, 6)
    roll2 = random.randint(1, 6)
    total = roll1 + roll2
    return total

def simulate(num, prisonOn):
    tab = init()
    tab[0] = 1
    place = 0

    for _ in range(num):
        roll = diceRoll()
        place += roll
        place = place % 40
        tab[place] += 1
        if prisonOn and place == 30:
            place = 10
            tab[place] += 1

    total_rolls = sum(tab.values())
    probabilities = {key + 1: value / total_rolls for key, value in tab.items()}
    return probabilities


def plotHistograms(results):
    fig, axs = plt.subplots(2, 2, figsize=(11.69, 8.27))
    for ax, (result_dict, title) in zip(axs.flat, results):
        ax.bar(result_dict.keys(), result_dict.values(), color="blue")
        ax.set_title(title)
        ax.set_xlabel("Pole")
        ax.set_ylabel("Liczba odwiedzin [%]")
        ax.set_xticks(range(0, 40, 5))
        ax.grid(axis='y', linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig("monopoly_distribution.jpg", format="jpg", dpi=300, bbox_inches="tight")
    plt.show()

def main():
    os.system("clear")

    cases = [
        (100, False, "100 rzutów - bez więzienia"),
        (1000000, False, "1 000 000 rzutów - bez więzienia"),
        (100, True, "100 rzutów - z więzieniem"),
        (1000000, True, "1 000 000 rzutów - z więzieniem"),
    ]

    results = [(simulate(num, prison_on), title) for num, prison_on, title in cases]

    plotHistograms(results)

if __name__ == "__main__":
    os.system("clear")
    main()