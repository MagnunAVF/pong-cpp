#!/usr/bin/env bash
set -euo pipefail

usage() {
    echo "Usage: $(basename "$0") {major|minor|patch}" >&2
    echo "Bumps the semver version in VERSION, commits it, and tags it." >&2
    exit 1
}

[ $# -eq 1 ] || usage

bump_type="$1"
case "$bump_type" in
    major | minor | patch) ;;
    *) usage ;;
esac

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
version_file="$repo_root/VERSION"

if [ -n "$(git -C "$repo_root" status --porcelain)" ]; then
    echo "Error: working tree is not clean. Commit or stash changes first." >&2
    exit 1
fi

current_version="$(cat "$version_file")"
IFS='.' read -r major minor patch <<< "$current_version"

case "$bump_type" in
    major)
        major=$((major + 1))
        minor=0
        patch=0
        ;;
    minor)
        minor=$((minor + 1))
        patch=0
        ;;
    patch)
        patch=$((patch + 1))
        ;;
esac

new_version="${major}.${minor}.${patch}"
echo "$new_version" > "$version_file"

git -C "$repo_root" add "$version_file"
git -C "$repo_root" commit -m "chore: bump version to v${new_version}"
git -C "$repo_root" tag -a "v${new_version}" -m "v${new_version}"

echo "Bumped version: ${current_version} -> ${new_version}"
echo "Created tag v${new_version}. Push with: git push && git push origin v${new_version}"
