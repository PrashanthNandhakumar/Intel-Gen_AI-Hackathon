## AI Enhanced Musume guide with Rover

### Project Description

The "AI Enhanced Museum Guide with Rover" project aims to revolutionize the visitor experience in museums by integrating advanced artificial intelligence (AI) technology with a mobile robotic guide. This innovative solution seeks to make museum visits more interactive, educational, and enjoyable for diverse audiences.

### Flow Diagram

<!-- <script src="https://cdn.jsdelivr.net/npm/mermaid@8.4.8/dist/mermaid.min.js"></script> -->

```mermaid
graph TD
    A[Visitor enters museum] --> B[Visitor borrows a rover]
    B --> C[Rover approaches an artifact]
    C --> D[Rover recognizes artifact]
    D --> E[Rover sends artifact ID to GenAI API]
    E --> F{GenAI API checks artifact ID in dictionary}
    F --> |Match found| G[Select appropriate prompt]
    G --> H[Send prompt to GenAI API]
    H --> I[GenAI API generates response]
    I --> J[Send response to Flask API]
    J --> K[Flask API serves response to rover's frontend]
    K --> L[Rover presents information to visitor]
    L --> M{Visitor has more questions?}
    M --> |Yes| N[Visitor asks question]
    N --> H
    M --> |No| O[Move to next artifact]
    O --> C
    F --> |No match| P[Use default prompt]
    P --> H
```