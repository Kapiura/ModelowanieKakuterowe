import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

plt.style.use('seaborn-v0_8')
A4_HORIZONTAL = (11.69, 8.27)
DPI = 300

def zapisz_png(nazwa):
    """Zapisuje aktualny wykres jako PNG"""
    plt.savefig(nazwa, dpi=DPI, bbox_inches='tight', format='png')
    plt.close()
    print(f"Zapisano {nazwa}")

# Zadanie 1 - Korelacja odległości
def zadanie1():
    dane = np.loadtxt('wyniki_zadanie1.txt')
    plt.figure(figsize=A4_HORIZONTAL)
    plt.plot(dane[:,0], dane[:,1], 'o', markersize=3, color='royalblue')
    plt.xlabel('Początkowa odległość')
    plt.ylabel('Skumulowana odległość')
    plt.title('Korelacja odległości')
    plt.grid(True, alpha=0.3)
    zapisz_png('zad1_korelacja.png')

# Zadanie 2 - Równanie logistyczne
def zadanie2():
    dane = np.loadtxt('wyniki_zadanie2.txt')
    plt.figure(figsize=A4_HORIZONTAL)
    plt.plot(dane[:,0], dane[:,1], 'o-', markersize=3, linewidth=1, color='crimson')
    plt.xlabel('Numer iteracji')
    plt.ylabel('Wartość x')
    plt.title('Równanie logistyczne')
    plt.grid(True, alpha=0.3)
    zapisz_png('zad2_logistyczne.png')

# Zadanie 3 - Atraktor Lorentza
def zadanie3():
    dane = np.loadtxt('wyniki_zadanie3.txt')
    fig = plt.figure(figsize=A4_HORIZONTAL)
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(dane[:,0], dane[:,1], dane[:,2], lw=0.5, color='purple')
    ax.set_title("Atraktor Lorentza")
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.grid(True, alpha=0.3)
    zapisz_png('zad3_lorentz.png')

# Zadanie 4 - Animacja wahadeł
def zadanie4():
    data = np.loadtxt('wyniki_zadanie4_animacja.txt')
    frames = int(np.max(data[:,0])) + 1
    num_pendulums = int(np.max(data[:,1])) + 1

    fig, ax = plt.subplots(figsize=(12, 12))
    ax.set_xlim(-2.5, 2.5)
    ax.set_ylim(-2.5, 2.5)
    ax.set_aspect('equal')
    ax.grid(True, alpha=0.3)

    cmap = plt.cm.get_cmap('hsv', num_pendulums)
    lines = [ax.plot([], [], '-', lw=1.5, color=cmap(i))[0]
             for i in range(num_pendulums)]

    ax.set_title(f'Animacja {num_pendulums} wahadeł podwójnych', pad=20)

    def update(frame):
        frame_data = data[data[:,0] == frame]
        for i, line in enumerate(lines):
            pend_data = frame_data[frame_data[:,1] == i][0]
            x = [0, pend_data[4], pend_data[6]]
            y = [0, pend_data[5], pend_data[7]]
            line.set_data(x, y)
        return lines

    ani = FuncAnimation(fig, update, frames=frames, interval=50, blit=True)
    ani.save('wahadla_animacja.mp4', writer='ffmpeg', fps=25, dpi=150)
    plt.close()
    print(f"Zapisano animację ({frames} klatek) do wahadla_animacja.mp4")

    for idx in [1, 99]:
        try:
            theta, omega = np.loadtxt(f'wyniki_zadanie4_faza_{idx}.txt', unpack=True)
            plt.figure(figsize=A4_HORIZONTAL)
            plt.plot(theta, omega, '-', lw=1, color='teal')
            plt.title(f'Przestrzeń fazowa wahadła {idx}')
            plt.xlabel('θ₂ [rad]')
            plt.ylabel('ω₂ [rad/s]')
            plt.grid(True, alpha=0.3)
            zapisz_png(f'zad4_faza_{idx}.png')
        except Exception as e:
            print(f"Błąd przy wahadle {idx}: {str(e)}")

if __name__ == "__main__":
    zadanie1()
    zadanie2()
    zadanie3()
    zadanie4()