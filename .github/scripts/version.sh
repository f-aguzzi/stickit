#!/bin/bash
set -e

echo "Debug: Checking for tags..."
LAST_TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "v0.0.0")
echo "Debug: LAST_TAG is $LAST_TAG"

if [ "$LAST_TAG" = "v0.0.0" ]; then
    echo "Debug: No tags found, using all commits."
    COMMITS=$(git log --pretty=format:"%s" 2>/dev/null)
else
    echo "Debug: Using commits since $LAST_TAG."
    COMMITS=$(git log "$LAST_TAG"..HEAD --pretty=format:"%s" 2>/dev/null)
fi

if [ -z "$COMMITS" ]; then
    echo "Error: No commits found."
    exit 1
fi

echo "Debug: COMMITS are:"
echo "$COMMITS"

IFS='.' read -r MAJOR MINOR PATCH <<< "${LAST_TAG#v}"

BUMP="none"

while IFS= read -r msg; do
  if echo "$msg" | grep -qiE "BREAKING[- ]CHANGE|^[a-z]+(\(.+\))?!:"; then
    BUMP="major"; break
  elif echo "$msg" | grep -qE "^feat(\(.+\))?:"; then
    [ "$BUMP" != "major" ] && BUMP="minor"
  elif echo "$msg" | grep -qE "^fix(\(.+\))?:"; then
    [ "$BUMP" = "none" ] && BUMP="patch"
  fi
done <<< "$COMMITS"

if [ "$BUMP" = "none" ]; then
  echo "No releasable commits. Skipping."
  echo "skipped=true" >> "$GITHUB_OUTPUT"
  exit 0
fi

case "$BUMP" in
  major) MAJOR=$((MAJOR+1)); MINOR=0; PATCH=0 ;;
  minor) MINOR=$((MINOR+1)); PATCH=0 ;;
  patch) PATCH=$((PATCH+1)) ;;
esac

NEXT_VERSION="v$MAJOR.$MINOR.$PATCH"
echo "Next version: $NEXT_VERSION"

git config user.name "github-actions"
git config user.email "github-actions@github.com"
git tag "$NEXT_VERSION"
git push origin "$NEXT_VERSION"

gh release create "$NEXT_VERSION" \
  --title "$NEXT_VERSION" \
  --generate-notes

echo "version=$NEXT_VERSION" >> "$GITHUB_OUTPUT"
