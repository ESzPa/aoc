#!/usr/bin/env bash

# Usage:
#   ./run.sh YEAR DAY 1  => runs part one
#   ./run.sh YEAR DAY 2  => runs part two
#   ./run.sh YEAR DAY    => runs both parts

set -e

YEAR=$1
DAY=$2
PART=$3

if [[ -z "$YEAR" || -z "$DAY" || ( -n "$PART" && "$PART" != "1" && "$PART" != "2" ) ]]; then
    echo "Usage: $0 <year> <day> [part]"
    echo "Example: $0 2024 5 1"
    exit 1
fi

ROOT_DIR="$(dirname "$0")"
YEAR_DIR="$ROOT_DIR/$YEAR"

print() {
    local text="$1"
    local color="$2"

    local RED='\033[0;31m'
    local GREEN='\033[0;32m'
    local YELLOW='\033[0;33m'
    local BLUE='\033[0;34m'
    local MAGENTA='\033[0;35m'
    local CYAN='\033[0;36m'
    local NC='\033[0m'

    local code="$NC"
    case "$color" in
        red) code="$RED" ;;
        green) code="$GREEN" ;;
        yellow) code="$YELLOW" ;;
        blue) code="$BLUE" ;;
        magenta) code="$MAGENTA" ;;
        cyan) code="$CYAN" ;;
        *) code="$NC" ;;
    esac

    echo -e "${code}${text}${NC}"
}
run_part() {
    local daypart=$1
    local realpart=$2
    local DIR="$YEAR_DIR/$daypart"
    local SRC="$DIR/main.cpp"
    local BIN="$DIR/main"

    if [[ ! -d "$DIR" ]]; then
        print "Directory not found: $DIR" red
        return
    fi

    g++ -std=c++17 -O2 -pipe -o "$BIN" "$SRC"

    print "$YEAR/$DAY:$realpart" green
    (
        cd "$DIR"
        ./main
    )
    print ""
}

if [[ -z "$PART" ]]; then
    run_part "${DAY}.0" "1"
    run_part "${DAY}.5" "2"
else
    if [[ "$PART" == 1 ]]; then
        run_part "$DAY.0" "1"
    else
        run_part "$DAY.5" "2"
    fi
fi
