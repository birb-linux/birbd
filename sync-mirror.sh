#!/bin/bash
set -e

# This script will go through all birb packages that it can
# find on the system (through the use of /etc/birb-sources.conf)
# and check if the given file exists. If it finds a file that is not
# on the system, it will download it to distcache.

DISTCACHE_PATH="/var/cache/distfiles"

# Birb funcs are required for sourcing seed files
BIRB_FUNCS_PATH="/usr/lib/birb/birb_funcs"
if [ ! -f "$BIRB_FUNCS_PATH" ]
then
	echo "The $BIRB_FUNCS_PATH file from the birb package manager is required for sourcing seeds"
	exit 1
fi

# shellcheck source=/usr/lib/birb/birb_funcs
source "$BIRB_FUNCS_PATH"

# Check if all script dependencies are on the system
[ ! -f /usr/bin/wget ] 		&& echo "Missing dependency: wget" 		&& exit 1
[ ! -f /usr/bin/md5sum ] 	&& echo "Missing dependency: md5sum" 	&& exit 1

# Get list of repositories
REPOS="$(sed '/^#/d; /^[[:space:]]*$/d' < /etc/birb-sources.conf)"

# Make sure that the distcache directory exists
mkdir -p "$DISTCACHE_PATH"

# Loop through the repositories
for REPO in $REPOS
do
	# Parse the path of the repository
	REPO_PATH="$(echo "$REPO" | cut -d';' -f3)"

	# Check if the repository exists
	if [ ! -d "$REPO_PATH" ]
	then
		echo "Skipping non-existent repository: $REPO_PATH"
		continue
	fi

	# Loop through the seed files in the repository
	SEED_FILES="$(find "$REPO_PATH" -iname "seed.sh" -print)"
	for SEED in $SEED_FILES
	do
		# Source the URL and checksum variables from the seed
		unset SOURCE, CHECKSUM
		# shellcheck source=/var/db/pkg/vim/seed.sh
		source "$SEED"

		# Parse the filename from the source URL
		TARBALL_NAME="$(basename "$SOURCE")"

		# Check if the tarball is in distcache
		if [ ! -f "$DISTCACHE_PATH/$TARBALL_NAME" ]
		then
			# Handle .onion addresses with torsocks if required
			TORSOCKS_COMMAND=""
			if onion_check "$SOURCE"
			then
				# The package has an onion source URL, check if tor is installed and running
				tor_check || exit 1

				# Make wget use torsocks
				TORSOCKS_COMMAND="torsocks"
				echo "Downloading the package through Tor from $SOURCE"
			fi

			# The tarball isn't in distcache, download it using wget
			$TORSOCKS_COMMAND wget -q --show-progress --directory-prefix="$DISTCACHE_PATH" "$SOURCE"

			# Check the integrity of the tarball
			TARBALL_CHECKSUM="$(md5sum "$DISTCACHE_PATH/$TARBALL_NAME" | cut -d ' ' -f1)"
			if [ "$CHECKSUM" != "$TARBALL_CHECKSUM" ]
			then
				echo "Skipping and removing corrupt tarball: $TARBALL_NAME"
				rm "$DISTCACHE_PATH/$TARBALL_NAME"
				continue
			fi
		fi
	done
done
