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

## Build production version

To build the production version, run the following:

```bash
hugo --gc --minify --baseURL
```

## Editing Hugo Configuration

The file `docs/hugo.yaml` controls the site's theme, navigation menu, and display options.

### Theme Module

Hugo loads the Hextra theme as a module dependency:

```yaml
module:
  imports:
    - path: github.com/imfing/hextra
```

Do not remove this block; it is required for the theme to load.

### Markup Settings

Under `markup:` you can configure Markdown rendering:

- `goldmark.renderer.unsafe: true` — allows raw HTML in markdown content (e.g., custom `<div>` elements). Set to `false` to strip unsafe tags.
- `highlight.noClasses: false` — enables Hextra's syntax highlighting styles. Change to `true` to output plain `<pre><code>` without class attributes.

### Navigation Menu

The `menu.main:` section defines the top navigation bar. Each entry has:

| Field     | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `name`    | Label shown in the navbar                                    |
| `pageRef` | Path to a page within `docs/content/` (use `/` for the root) |
| `url`     | External URL (overrides `pageRef`)                           |
| `weight`  | Sort order; lower values appear first                        |
| `params`  | Extra options, e.g. `type: search` for the search button or `icon: github` for an icon |

To add a new menu item:

```yaml
menu:
  main:
    - name: New Section
      pageRef: /new_section
      weight: 7
```

Ensure the corresponding page file exists under `docs/content/new_section/`.

### Display Parameters

Under `params:` you can control visual aspects:

- `navbar.displayTitle` — show the site title in the navbar
- `navbar.displayLogo` — show the logo in the navbar
- `footer.displayCopyright` — show a copyright notice in the footer
- `footer.displayPoweredBy` — show "Powered by Hugo & Hextra" in the footer

All params support `true` or `false` toggles. Refer to the [Hextra documentation](https://imfing.github.io/hextra) for the full list of available options.
