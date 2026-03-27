import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors

def generate_heatmap(df, hand_type, filename):
    # Filter the dataset for Hard or Soft totals
    subset = df[df['HandType'] == hand_type].copy()

    # Map the actions to integers for the color map: S=0, H=1, D=2
    action_map = {'S': 0, 'H': 1, 'D': 2}
    subset['ActionCode'] = subset['OptimalMove'].map(action_map)
    
    # Pivot to create the 2D grid
    matrix = subset.pivot(index='PlayerTotal', columns='DealerCard', values='ActionCode')
    matrix = matrix.sort_index(ascending=False) # Highest totals at the top

    plt.figure(figsize=(10, 8))
    
    # Custom colors: Red (Stand), Green (Hit), Blue (Double)
    cmap = mcolors.ListedColormap(['#ff4c4c', '#4caf50', '#2196F3'])
    
    # Draw heatmap
    ax = sns.heatmap(matrix, cmap=cmap, cbar=False, linewidths=0.5, linecolor='black')
    
    ax.set_title(f'Monte Carlo GTO Strategy - {hand_type} Totals', fontsize=16, pad=20)
    ax.set_xlabel('Dealer Upcard', fontsize=12)
    ax.set_ylabel('Player Total', fontsize=12)
    
    xticks = [str(x) if x != 11 else 'A' for x in matrix.columns]
    ax.set_xticklabels(xticks)

    # Overlay text annotations
    for i in range(matrix.shape[0]):
        for j in range(matrix.shape[1]):
            action_code = matrix.iloc[i, j]
            # Reverse lookup the string from the code
            text = list(action_map.keys())[list(action_map.values()).index(action_code)]
            ax.text(j + 0.5, i + 0.5, text, 
                    ha='center', va='center', color='white', 
                    fontsize=12, fontweight='bold')

    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    print(f"Heatmap saved as '{filename}'.")
    plt.close()

def main():
    print("Ingesting C++ simulation data...")
    try:
        df = pd.read_csv('../basic_strategy.csv')
    except FileNotFoundError:
        print("Error: basic_strategy.csv not found.")
        return

    generate_heatmap(df, 'Hard', 'gto_hard_totals.png')
    generate_heatmap(df, 'Soft', 'gto_soft_totals.png')

if __name__ == "__main__":
    main()