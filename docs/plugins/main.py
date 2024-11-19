from pathlib import Path
import re
from jinja2 import Template
from mkdocs.config import Config

from mkdocs.structure.files import Files
from mkdocs.structure.pages import Page
import yaml

THIS_DIR = Path(__file__).parent


def on_page_markdown(markdown: str, page: Page, config: Config, files: Files) -> str:
    """
    Called on each file after it is read and before it is converted to HTML.
    """

    if md := populate_people(markdown, page):
        return md
    return markdown


experts_template = Template(
    """
<div class="user-list user-list-center">
    {% for user in people.experts %}
    <div class="user">
        <a href="{{ user.url }}" target="_blank">
            <div class="avatar-wrapper">
                <img src="{{ user.avatarUrl }}"/>
            </div>
            <div class="title">@{{ user.login }}</div>
        </a>
        <div class="count">Questions replied: {{ user.count }}</div>
    </div>
    {% endfor %}
</div>
"""
)

most_active_users_template = Template(
    """

<div class="user-list user-list-center">
    {% for user in people.last_month_active %}
    <div class="user">
        <a href="{{ user.url }}" target="_blank">
            <div class="avatar-wrapper">
                <img src="{{ user.avatarUrl }}"/>
            </div>
            <div class="title">@{{ user.login }}</div>
        </a>
        <div class="count">Questions replied: {{ user.count }}</div>
    </div>
    {% endfor %}
</div>
"""
)

top_contributors_template = Template(
    """
<div class="user-list user-list-center">
    {% for user in people.top_contributors %}
    <div class="user">
        <a href="{{ user.url }}" target="_blank">
            <div class="avatar-wrapper">
                <img src="{{ user.avatarUrl }}"/>
            </div>
            <div class="title">@{{ user.login }}</div>
        </a>
        <div class="count">Contributions: {{ user.count }}</div>
    </div>
    {% endfor %}
</div>
"""
)

top_reviewers_template = Template(
    """
<div class="user-list user-list-center">
    {% for user in people.top_reviewers %}
    <div class="user">
        <a href="{{ user.url }}" target="_blank">
            <div class="avatar-wrapper">
                <img src="{{ user.avatarUrl }}"/>
            </div>
            <div class="title">@{{ user.login }}</div>
        </a>
        <div class="count">Reviews: {{ user.count }}</div>
    </div>
    {% endfor %}
</div>
"""
)

maintainers_template = Template(
    """
<div class="user-list user-list-center">
    {% for user in people.maintainers %}
    <div class="user">
        <a href="{{ user.url }}" target="_blank">
            <div class="avatar-wrapper">
                <img src="{{ user.avatarUrl }}"/>
            </div>
            <div class="title">@{{ user.login }}</div>
        </a>
    </div>
    {% endfor %}
</div>
"""
)


def populate_people(markdown: str, page: Page) -> str | None:
    if page.file.src_uri != "people.md":
        return None

    # read people.yml file data
    with (THIS_DIR / "people.yml").open("rb") as f:
        people = yaml.load(f, Loader=yaml.FullLoader)

    # Render the templates
    for name, template in [
        ("experts", experts_template),
        ("most_active_users", most_active_users_template),
        ("top_contributors", top_contributors_template),
        ("top_reviewers", top_reviewers_template),
        ("maintainers", maintainers_template),
    ]:
        rendered = template.render(people=people)
        markdown = re.sub(f"{{{{ {name} }}}}", rendered, markdown)

    return markdown
