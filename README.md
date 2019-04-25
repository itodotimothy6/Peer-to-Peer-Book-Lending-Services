# Peer-to-Peer-Book-Lending-Services
## UNCF/PROSKY Challenge
### Create the backend architecture for a peer-to-peer book lending service.

- LEVEL 1: Book owners must be able to register a list of shared books.

- LEVEL 2: Help users to keep track of who has each book, return deadlines, and past sharing history.

- LEVEL 3: The system will allow users to request any available book. They must return them within a 30-days period.

- LEVEL 4: We need to provide a mechanism to calculate a user score. Users will be evaluated based on their actions:
  - a. Registering a book: +1
  - b. Lending a book: +5
  - c. Returning a book in good condition: +2
  - d. If the book is returned in bad condition: -10
  - e. Daily penalty for missing the deadline: -2
  
- LEVEL 5: Create a search service, allowing users to filter available books by name, author, or year.

- LEVEL 6: Provide a book ranking service, that returns the top N books based on their history. Each book score is calculated using their interactions with users.
