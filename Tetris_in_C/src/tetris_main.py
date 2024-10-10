import pygame
import sys

# Define constants for block colors
COLORS = {
    1: (255, 0, 0),  # Red for example
    2: (0, 255, 0),  # Green
    # Add more color mappings as needed
}

def display_board(window, game, block_size):
    window.fill((0, 0, 0))  # Clear screen (black background)

    for i in range(game.rows):
        for j in range(game.cols):
            if game.board[i][j] != 0:  # Non-empty block (assuming 0 is empty)
                color = COLORS.get(game.board[i][j], (255, 255, 255))  # Default to white if no color defined
                pygame.draw.rect(window, color, (j * block_size, i * block_size, block_size, block_size))

    pygame.display.update()  # Refresh the display
    
    
def display_piece(window, block, block_size):
    # Clear the window where the piece is displayed (if separate from the game board)
    window.fill((0, 0, 0))  # Black background

    if block.type == -1:
        pygame.display.update()
        return

    for c in block.get_positions():  # Assuming get_positions() returns the current positions of the piece's blocks
        pygame.draw.rect(window, COLORS[block.type], (c[1] * block_size, c[0] * block_size, block_size, block_size))

    pygame.display.update()
    
    
def display_score(window, game, font):
    window.fill((0, 0, 0))  # Clear the score window (black background)
    
    # Render the score
    score_text = font.render(f"Score: {game.points}", True, (255, 255, 255))  # White text
    level_text = font.render(f"Level: {game.level}", True, (255, 255, 255))
    lines_text = font.render(f"Lines: {game.lines_remaining}", True, (255, 255, 255))

    # Display score, level, and lines
    window.blit(score_text, (10, 10))  # Adjust coordinates based on your layout
    window.blit(level_text, (10, 40))
    window.blit(lines_text, (10, 70))

    pygame.display.update()
    

def save(game, window, font):
    running = True
    prompt_text = font.render("Save and exit (Y/N)?", True, (255, 255, 255))

    # Clear the window and display the prompt
    window.fill((0, 0, 0))  # Clear screen (black background)
    window.blit(prompt_text, (50, 100))  # Display prompt at a specific position
    pygame.display.update()

    # Event loop for handling the Y/N response
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_n:
                    # If the user chooses not to save, exit this function
                    return
                elif event.key == pygame.K_y:
                    # Save the game to a file
                    with open("tetris.save", "w") as f:
                        tg_save(game, f)  # You'd need to define how to save the game data
                    print("Game saved to 'tetris.save'.")
                    print("Resume by passing the filename as an argument to this program.")
                    
                    # Properly end the game and quit
                    tg_delete(game)  # Delete game state, similar to the original
                    pygame.quit()
                    sys.exit()

        # Optional small delay to avoid high CPU usage in the loop
        pygame.time.wait(100)