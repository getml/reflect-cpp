# Building and Viewing the Documentation

The documentation is built with [Hugo](https://gohugo.io/) using the [Hextra](https://github.com/imfing/hextra) theme.

## Prerequisites

- [Hugo Extended](https://gohugo.io/installation/) (version 0.124 or later)

On macOS:

```bash
brew install hugo
```

On Windows (via winget):

```bash
winget install Hugo.Hugo
```

On Linux, follow the [official installation guide](https://gohugo.io/installation/).

## Building and Viewing

```bash
cd docs

# Install dependencies
hugo mod tidy

# Start the development server
hugo server --disableFastRender -p 1313
```

The documentation will be available at [http://localhost:1313](http://localhost:1313).

Hugo watches for file changes and automatically re-renders the site. Simply refresh your browser to see updates.
