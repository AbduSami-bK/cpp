#!/usr/bin/env bash
set -e

if ! command -v gh &>/dev/null
then
	echo "GitHub CLI (gh) not found. Install it first."
	exit 1
fi

read -p "Repo name: " REPO
gh repo create "$REPO" --public --source=. --push
