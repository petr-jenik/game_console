import pygame
import sys
import random
import math

# Constants
WIDTH, HEIGHT = 800, 600
FPS = 60
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
RED = (255, 0, 0)
BLACK = (0, 0, 0)
YELLOW = (255,255,0)
# Player class
class Player(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface((50, 50))
        self.image.fill(BLUE)
        self.rect = self.image.get_rect()
        self.rect.center = (50, HEIGHT // 2)  #WIDTH // 2, HEIGHT - 50)
        self.speed = 5
        self.health = 3000000000

    def update(self):
        keys = pygame.key.get_pressed()
        if keys[pygame.K_UP] and self.rect.top > 0:
            self.rect.y -= self.speed
        if keys[pygame.K_DOWN] and self.rect.bottom < HEIGHT:
            self.rect.y += self.speed
        if keys[pygame.K_LEFT] and self.rect.left > 10:
            self.rect.x -= self.speed
        if keys[pygame.K_RIGHT] and self.rect.right < WIDTH // 2:
            self.rect.x += self.speed


    def hit(self):
        self.show_explosion = True
        self.health -= 1
        if self.health <= 0:
            pygame.quit()
            sys.exit()

    def draw(self, screen):
        pygame.draw.rect(screen, WHITE, self.rect)

        #if self.show_explosion:
        #    pygame.draw.circle(screen, RED, self.rect, 10,2)


# Star class
class Star(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.speed = random.choice((1, 2, 3)) / 3
        # Speed is also a size

        size = 3 * self.speed

        self.image = pygame.Surface((size, size))
        self.image.fill(WHITE)
        self.rect = self.image.get_rect()
        self.rect.topleft = (random.randint(0, 2 * WIDTH), random.randint(0, HEIGHT))
        self.x = self.rect.x

    def update(self):
        self.x -= self.speed
        self.rect.x = self.x

        if self.rect.right < 0:
            self.rect.topleft = (WIDTH, random.randint(0, HEIGHT))
            self.x = self.rect.x
            #self.speed = random.randint(1, 3)

    def draw(self, screen):
        pygame.draw.rect(screen, WHITE, self.rect)


MOVEMENT_LINE = 1
MOVEMENT_WAVE = 2

# Enemy class
class Enemy(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface((30, 30))
        self.image.fill(RED)
        self.rect = self.image.get_rect()
        self.restart()

    def restart(self):
        self.rect.topleft = (WIDTH + random.randint(30, 50), random.randint(0, HEIGHT - 30))
        self.speed = random.randint(2, 4)
        self.old_y = self.rect.y
        self.movement_type = random.choice((MOVEMENT_WAVE, MOVEMENT_LINE, MOVEMENT_LINE, MOVEMENT_LINE))

    def update(self):
        self.rect.x -= self.speed

        if self.movement_type == MOVEMENT_WAVE:
            self.rect.y = self.old_y + 20 * math.sin(math.radians(self.rect.x))  + 30* math.cos(math.radians(self.rect.x))

        if self.rect.right < 0:
            self.restart()

    def draw(self, screen):
        pygame.draw.rect(screen, WHITE, self.rect)



# Projectile class
class Projectile(pygame.sprite.Sprite):
    def __init__(self, x, y, speed):
        super().__init__()
        self.image = pygame.Surface((10, 5))
        self.image.fill(YELLOW)
        self.rect = self.image.get_rect()
        self.rect.midbottom = (x, y)
        self.speed = speed

    def update(self):
        self.rect.x += self.speed
        if self.rect.right < 0 or self.rect.left > WIDTH:
            self.kill()

    def draw(self, screen):
        pygame.draw.rect(screen, YELLOW, self.rect)

# EnemyProjectile class
class EnemyProjectile(pygame.sprite.Sprite):
    def __init__(self, x, y, speed):
        super().__init__()
        self.image = pygame.Surface((10, 5))
        self.image.fill(YELLOW)
        self.rect = self.image.get_rect()
        self.rect.midtop = (x, y)
        self.speed = speed

    def update(self):
        self.rect.x -= self.speed
        if self.rect.right < 0 or self.rect.left > WIDTH:
            self.kill()

    def draw(self, screen):
        pygame.draw.rect(screen, YELLOW, self.rect)

# Main game loop
def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    clock = pygame.time.Clock()

    player = Player()
    enemies = pygame.sprite.Group()
    projectiles = pygame.sprite.Group()
    enemy_projectiles = pygame.sprite.Group()

    stars = pygame.sprite.Group()


    all_sprites = pygame.sprite.Group()

    for _ in range(200):
        star = Star()
        stars.add(star)
        all_sprites.add(star)

    all_sprites.add(player)

    for _ in range(5):  # Initial enemy spawn
        enemy = Enemy()
        enemies.add(enemy)
        all_sprites.add(enemy)


    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    projectile = Projectile(player.rect.right, player.rect.centery, 5)
                    projectiles.add(projectile)
                    all_sprites.add(projectile)

        player.update()
        enemies.update()
        projectiles.update()
        enemy_projectiles.update()
        stars.update()

        explosions = []

        # Collision detection logic for player and enemy projectiles
        for enemy_projectile in pygame.sprite.spritecollide(player, enemy_projectiles, False):
            player.hit()
            enemy_projectile.kill()


        # Collision detection logic for projectiles and enemies
        for projectile in pygame.sprite.groupcollide(projectiles, enemies, True, True):
            enemy = Enemy()
            enemies.add(enemy)
            all_sprites.add(enemy)

        # Enemy firing logic
        for enemy in enemies:
            if random.randint(1, 100) <= 1:  # Adjust the probability of firing
                enemy_projectile = EnemyProjectile(enemy.rect.left, enemy.rect.centery, 5)
                enemy_projectiles.add(enemy_projectile)
                all_sprites.add(enemy_projectile)

        screen.fill(BLACK)  # Fill the screen with black
        all_sprites.draw(screen)

        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
