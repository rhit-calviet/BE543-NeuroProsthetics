import pygame

# Initialize pygame
pygame.init()

# Define window and player size
WINDOW_SIZE = 600
PLAYER_SIZE = 20
window = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
pygame.display.set_caption("Free Movement Maze Game")

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)

# Initial player position
player_pos = [50, 50]

# Define walls as rectangles (x, y, width, height)
walls = [
    pygame.Rect(100, 100, 400, 20),
    pygame.Rect(100, 200, 300, 20),
    pygame.Rect(300, 300, 200, 20),
    pygame.Rect(200, 400, 100, 20)
]

goal_pos = pygame.Rect(550, 550, 30, 30)

def draw_maze():
    window.fill(WHITE)

    # Draw player
    pygame.draw.rect(window, BLUE, (player_pos[0], player_pos[1], PLAYER_SIZE, PLAYER_SIZE))
    
    # Draw walls
    for wall in walls:
        pygame.draw.rect(window, BLACK, wall)

    # Draw goal
    pygame.draw.rect(window, GREEN, goal_pos)
    
    pygame.display.flip()

def check_collision(new_pos):
    player_rect = pygame.Rect(new_pos[0], new_pos[1], PLAYER_SIZE, PLAYER_SIZE)
    for wall in walls:
        if player_rect.colliderect(wall):
            return True
    return False

def move_player(dx, dy):
    new_pos = [player_pos[0] + dx, player_pos[1] + dy]

    if 0 <= new_pos[0] <= WINDOW_SIZE - PLAYER_SIZE and 0 <= new_pos[1] <= WINDOW_SIZE - PLAYER_SIZE:
        if not check_collision(new_pos):
            player_pos[0] = new_pos[0]
            player_pos[1] = new_pos[1]

# Main game loop
running = True
clock = pygame.time.Clock()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Movement keys with diagonal support
    keys = pygame.key.get_pressed()
    dx, dy = 0, 0

    if keys[pygame.K_UP]:
        dy = -1
    if keys[pygame.K_DOWN]:
        dy = 1
    if keys[pygame.K_LEFT]:
        dx = -1
    if keys[pygame.K_RIGHT]:
        dx = 1

    move_player(dx, dy)

    # Check for win condition
    if pygame.Rect(player_pos[0], player_pos[1], PLAYER_SIZE, PLAYER_SIZE).colliderect(goal_pos):
        print("You Win!")
        running = False

    draw_maze()
    clock.tick(60)

pygame.quit()
