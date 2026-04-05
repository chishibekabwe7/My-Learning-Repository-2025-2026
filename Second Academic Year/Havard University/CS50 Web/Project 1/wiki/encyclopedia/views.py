import random

import markdown2
from django.shortcuts import redirect, render

from . import util


def _find_entry_title_case_insensitive(title):
    for entry in util.list_entries():
        if entry.lower() == title.lower():
            return entry
    return None


def index(request):
    return render(request, "encyclopedia/index.html", {
        "entries": util.list_entries()
    })


def entry_page(request, title):
    normalized_title = _find_entry_title_case_insensitive(title)
    if not normalized_title:
        return render(request, "encyclopedia/error.html", {
            "title": "Page Not Found",
            "message": f"The requested entry '{title}' was not found."
        }, status=404)

    markdown_content = util.get_entry(normalized_title)
    html_content = markdown2.markdown(markdown_content)
    return render(request, "encyclopedia/entry.html", {
        "title": normalized_title,
        "content": html_content
    })


def search(request):
    query = request.GET.get("q", "").strip()
    entries = util.list_entries()

    if not query:
        return render(request, "encyclopedia/search.html", {
            "query": query,
            "results": []
        })

    exact_match = _find_entry_title_case_insensitive(query)
    if exact_match:
        return redirect("entry", title=exact_match)

    results = [entry for entry in entries if query.lower() in entry.lower()]
    return render(request, "encyclopedia/search.html", {
        "query": query,
        "results": results
    })


def new_page(request):
    if request.method == "POST":
        title = request.POST.get("title", "").strip()
        content = request.POST.get("content", "")

        if not title:
            return render(request, "encyclopedia/new.html", {
                "error": "Title is required.",
                "title_input": title,
                "content_input": content
            })

        if _find_entry_title_case_insensitive(title):
            return render(request, "encyclopedia/new.html", {
                "error": "An entry with this title already exists.",
                "title_input": title,
                "content_input": content
            })

        util.save_entry(title, content)
        return redirect("entry", title=title)

    return render(request, "encyclopedia/new.html")


def edit_page(request, title):
    normalized_title = _find_entry_title_case_insensitive(title)
    if not normalized_title:
        return render(request, "encyclopedia/error.html", {
            "title": "Page Not Found",
            "message": f"The requested entry '{title}' was not found."
        }, status=404)

    if request.method == "POST":
        content = request.POST.get("content", "")
        util.save_entry(normalized_title, content)
        return redirect("entry", title=normalized_title)

    content = util.get_entry(normalized_title)
    return render(request, "encyclopedia/edit.html", {
        "title": normalized_title,
        "content": content
    })


def random_page(request):
    entries = util.list_entries()
    if not entries:
        return render(request, "encyclopedia/error.html", {
            "title": "No Entries",
            "message": "No encyclopedia entries are available yet."
        }, status=404)

    random_title = random.choice(entries)
    return redirect("entry", title=random_title)

