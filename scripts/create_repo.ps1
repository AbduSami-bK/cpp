param([string]$RepoName)

if (-not (Get-Command gh -ErrorAction SilentlyContinue)) {
	Write-Error "GitHub CLI (gh) not found."
	exit 1
}

gh repo create $RepoName --public --source=. --push
