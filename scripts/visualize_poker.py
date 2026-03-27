import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

def generate_poker_heatmap():
    print("Ingesting C++ simulation data...")
    try:
        # Go up one directory to find the CSV built by the C++ engine
        df = pd.read_csv('../build/preflop_equity.csv')
    except FileNotFoundError:
        try:
            df = pd.read_csv('preflop_equity.csv') # Fallback if run from root
        except:
            print("Error: preflop_equity.csv not found.")
            return

    # Create a 13x13 empty matrix
    ranks = list("AKQJT98765432")
    matrix = pd.DataFrame(index=ranks, columns=ranks, dtype=float)

    # Populate the matrix
    for _, row in df.iterrows():
        hand = row['Hand']
        equity = row['Equity'] * 100 # Convert to percentage
        
        r1, r2 = hand[0], hand[1]
        
        if len(hand) == 2:    # Pairs go on the diagonal
            matrix.loc[r1, r2] = equity
        elif hand[2] == 's':  # Suited hands go in the upper-right triangle
            matrix.loc[r1, r2] = equity
        elif hand[2] == 'o':  # Offsuit hands go in the lower-left triangle
            matrix.loc[r2, r1] = equity 

    # Set up the visualization
    plt.figure(figsize=(12, 10))
    
    # Use a coolwarm color map (Red = high equity, Blue = low equity)
    ax = sns.heatmap(matrix, cmap="coolwarm", annot=True, fmt=".1f", 
                     linewidths=0.5, cbar_kws={'label': 'Expected Equity (%)'})
    
    ax.set_title("Monte Carlo Preflop Equity (Random Opponent)", fontsize=18, pad=20)
    plt.tight_layout()
    
    plt.savefig('preflop_equity_heatmap.png', dpi=300)
    print("Heatmap saved as 'preflop_equity_heatmap.png'.")
    plt.show()

if __name__ == "__main__":
    generate_poker_heatmap()