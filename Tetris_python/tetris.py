import pygame
import random
#import time
#import os

# Constants
WIDTH, HEIGHT = 160, 320
BLOCK_SIZE = 16
COLS = WIDTH // BLOCK_SIZE
ROWS = HEIGHT// BLOCK_SIZE
SIDEBAR_WIDTH = 80
FPS = 30

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
ORANGE = (255, 165, 0)
YELLOW = (255, 255, 0)
GREEN = (0, 255, 0)
PURPLE = (128, 0, 128)
RED = (255, 0, 0)
GREY = (128, 128, 128)


# Tetromino shapes and colors
TETROMINOS = [
    [[(0, 0), (1, 0), (2, 0), (3, 0)], CYAN],  # I
    [[(0, 0), (1, 0), (2, 0), (2, 1)], ORANGE], # L
    [[(0, 1), (1, 1), (2, 1), (2, 0)], BLUE],  # J
    [[(0, 0), (1, 0), (1, 1), (2, 1)], GREEN],  # S
    [[(0, 0), (0, 1), (1, 0), (1, 1)], YELLOW], # O
    [[(0, 1), (1, 1), (2, 1), (1, 0)], PURPLE], # T
    [[(0, 1), (1, 1), (1, 0), (2, 0)], RED]    # Z
]

# Gravity levels (ticks per fall) - converted from C array
GRAVITY_LEVEL = [
# 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
  50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
#10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  30, 28, 26, 24, 22, 20, 16, 12,  8,  4
]

class Block:
    def __init__(self, x, y, type):
        self.x = x
        self.y = y
        self.type = type
        self.shape = TETROMINOS[type][0]  # Initial orientation
        self.color = TETROMINOS[type][1]

    def rotate(self, board):
        original_shape = self.shape
        rotated_shape = []
        for x, y in self.shape:
            rotated_shape.append((y, -x))

        # Check for collision after rotation
        if not self.check_collision(rotated_shape, board):
            self.shape = rotated_shape
        else:
            self.shape = original_shape


    def move(self, dx, dy, board):
        if not self.check_collision([(x + dx, y + dy) for x, y in self.shape], board):
            self.x += dx
            self.y += dy

    def check_collision(self, shape, board):
        for x, y in shape:
            px = self.x + x
            py = self.y + y
            if px < 0 or px >= COLS or py >= ROWS or (py >= 0 and board[py][px]):
                return True
        return False
    
    def draw(self, screen):
        for x, y in self.shape:
            pygame.draw.rect(screen, self.color, (BLOCK_SIZE * (self.x + x), BLOCK_SIZE * (self.y + y), BLOCK_SIZE, BLOCK_SIZE))
            pygame.draw.rect(screen, GREY, (BLOCK_SIZE * (self.x + x), BLOCK_SIZE * (self.y + y), BLOCK_SIZE, BLOCK_SIZE),1)


class Tetris:

    def __init__(self):
         pygame.init()
         self.screen = pygame.display.set_mode((WIDTH + SIDEBAR_WIDTH, HEIGHT))
         pygame.display.set_caption("Tetris")
         self.clock = pygame.time.Clock()
         self.font = pygame.font.Font(None, 30)

         #Initial game values
         self.board = [[0 for _ in range(COLS)] for _ in range(ROWS)]
         self.falling_block = self.get_new_block()
         self.next_block = self.get_new_block()
         self.stored_block = None
         self.can_store = True
         self.score = 0
         self.level = 0
         self.lines_remaining = 10
         self.ticks_til_gravity = GRAVITY_LEVEL[self.level]
         self.game_over = False
         self.paused = False



    def run(self):
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                   pygame.quit()
                   return
                if event.type == pygame.KEYDOWN:

                    if self.game_over:
                        if event.key == pygame.K_r:
                           self.reset_game()
                    elif not self.paused:
                        if event.key == pygame.K_LEFT:
                            self.falling_block.move(-1, 0, self.board)
                        if event.key == pygame.K_RIGHT:
                            self.falling_block.move(1, 0, self.board)
                        if event.key == pygame.K_UP:
                            self.falling_block.rotate(self.board)
                        if event.key == pygame.K_DOWN:
                           self.falling_block.move(0, 1, self.board)
                        if event.key == pygame.K_SPACE:
                           self.swap_hold()
                        if event.key == pygame.K_p:
                           self.paused = True 


                    elif event.key == pygame.K_p:
                        self.paused = False






            if not self.game_over and not self.paused:
               self.tick()

            self.draw()
            pygame.display.flip()
            self.clock.tick(FPS)


    def swap_hold(self):
        if self.can_store:
           if self.stored_block is None:
               self.stored_block = self.falling_block
               self.falling_block = self.next_block
               self.next_block = self.get_new_block()
           else:
               self.stored_block, self.falling_block = self.falling_block, self.stored_block
               self.falling_block.x = 4  # Reset position of swapped block
               self.falling_block.y = 0

           self.can_store = False


    def tick(self):
         self.ticks_til_gravity -= 1
         if self.ticks_til_gravity <= 0:
            self.move_block_down()
            self.ticks_til_gravity = GRAVITY_LEVEL[self.level]


    def move_block_down(self):
         if not self.falling_block.check_collision([(x, y+1) for x, y in self.falling_block.shape], self.board):
              self.falling_block.move(0,1,self.board)

         else:

            self.place_block()
            self.clear_lines()
            self.falling_block = self.next_block
            self.next_block = self.get_new_block()
            self.can_store = True

            if self.falling_block.check_collision(self.falling_block.shape, self.board):
                 self.game_over = True


    def draw_grid(self):
        for x in range(COLS):
            for y in range(ROWS):
                pygame.draw.rect(self.screen, GREY, (x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), 1)

    def draw_sidebar(self):
        pygame.draw.rect(self.screen, BLACK, (WIDTH, 0, SIDEBAR_WIDTH, HEIGHT))

        # Next block
        next_text = self.font.render("Next:", True, WHITE)
        self.screen.blit(next_text, (WIDTH, 20))
        if self.next_block:
            for x, y in self.next_block.shape:
                pygame.draw.rect(self.screen, self.next_block.color, (BLOCK_SIZE * (x + 12), BLOCK_SIZE * (y + 3), BLOCK_SIZE, BLOCK_SIZE))
                pygame.draw.rect(self.screen, GREY, (BLOCK_SIZE * (x + 12), BLOCK_SIZE * (y+3), BLOCK_SIZE, BLOCK_SIZE), 1)

        # Hold Block
        hold_text = self.font.render("Hold:", True, WHITE)
        self.screen.blit(hold_text,(WIDTH ,140))

        if self.stored_block:
            for x, y in self.stored_block.shape:
                pygame.draw.rect(self.screen, self.stored_block.color, (BLOCK_SIZE * (x + 12), BLOCK_SIZE * (y + 11), BLOCK_SIZE, BLOCK_SIZE))
                pygame.draw.rect(self.screen, GREY, (BLOCK_SIZE * (x + 12), BLOCK_SIZE * (y + 11), BLOCK_SIZE, BLOCK_SIZE), 1)

        # Score
        score_text = self.font.render(f"Score: {self.score}", True, WHITE)
        self.screen.blit(score_text, (WIDTH , 250))

        # Level
        level_text = self.font.render(f"Level: {self.level}", True, WHITE)
        self.screen.blit(level_text, (WIDTH , 300))


    def place_block(self):
        for x, y in self.falling_block.shape:
           self.board[self.falling_block.y + y][self.falling_block.x + x] = self.falling_block.color



    def clear_lines(self):

       lines_cleared = 0
       for y in range(ROWS):
           if all(self.board[y]):
               lines_cleared += 1

               for yy in range(y, 0, -1):
                   self.board[yy] = self.board[yy-1]
               self.board[0] = [0 for _ in range(COLS)]

       self.update_score_lines(lines_cleared)



    def update_score_lines(self, cleared_lines):
        if cleared_lines > 0:
            self.score +=  [0, 40, 100, 300, 1200][cleared_lines] * (self.level + 1)
            self.lines_remaining -= cleared_lines

            if self.lines_remaining <= 0:
               self.level += 1
               self.lines_remaining += 10
               if self.level < len(GRAVITY_LEVEL):
                   self.ticks_til_gravity = GRAVITY_LEVEL[self.level]


    def get_new_block(self):
         return Block(4, 0, random.randint(0, len(TETROMINOS) - 1))




    def draw_board(self):
        for y in range(ROWS):
            for x in range(COLS):
               if self.board[y][x]:
                   pygame.draw.rect(self.screen, self.board[y][x], (BLOCK_SIZE * x, BLOCK_SIZE * y, BLOCK_SIZE, BLOCK_SIZE))
                   pygame.draw.rect(self.screen, GREY, (BLOCK_SIZE * x, BLOCK_SIZE * y, BLOCK_SIZE, BLOCK_SIZE), 1)





    def draw(self):
         self.screen.fill(BLACK)
         self.draw_board()

         if self.falling_block:
            self.falling_block.draw(self.screen)

         self.draw_grid()
         self.draw_sidebar()
         
         if self.paused:
             pause_surface = pygame.Surface((WIDTH, HEIGHT), pygame.SRCALPHA)
             pause_surface.fill((0, 0, 0, 128))  # Semi-transparent black
             self.screen.blit(pause_surface, (0, 0))
             pause_text = self.font.render("PAUSED", True, WHITE)
             text_rect = pause_text.get_rect(center=(WIDTH // 2, HEIGHT // 2))
             self.screen.blit(pause_text, text_rect)


         if self.game_over:
              game_over_surface = pygame.Surface((WIDTH, HEIGHT), pygame.SRCALPHA)
              game_over_surface.fill((0, 0, 0, 128))  # Semi-transparent black
              self.screen.blit(game_over_surface, (0, 0))

              game_over_text = self.font.render("GAME OVER", True, WHITE)
              restart_text = self.font.render("Press 'r' to restart", True, WHITE)

              game_over_rect = game_over_text.get_rect(center=(WIDTH//2, HEIGHT//2 - 40))
              restart_rect = restart_text.get_rect(center=(WIDTH//2, HEIGHT//2 + 40))

              self.screen.blit(game_over_text,game_over_rect)
              self.screen.blit(restart_text,restart_rect)


    def reset_game(self):
         self.board = [[0 for _ in range(COLS)] for _ in range(ROWS)]
         self.falling_block = self.get_new_block()
         self.next_block = self.get_new_block()
         self.score = 0
         self.level = 0
         self.lines_remaining = 10
         self.ticks_til_gravity = GRAVITY_LEVEL[self.level]
         self.game_over = False




if __name__ == "__main__":
    game = Tetris()
    game.run()