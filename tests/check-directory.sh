#!/bin/sh
set -eu
binary=$(realpath "${1:-build/mugi-grep}")
work=$(mktemp -d)
trap '[ "$?" -eq 0 ] && rm -rf "$work" || echo "Test files retained: $work" >&2' EXIT
mkdir -p "$work/data/mugi-grep" "$work/folder with spaces"
export XDG_DATA_HOME="$work/data"
export QT_QPA_PLATFORM=offscreen
"$binary" --help | grep -q '\[directory\]'
if "$binary" "$work/missing"; then exit 1; fi
if "$binary" "$work" "$work"; then exit 1; fi
printf '{"sessions":[{"path":"/previous-folder"}]}' > "$work/data/mugi-grep/settings.json"
unset QT_QPA_PLATFORM SESSION_MANAGER
dbus-run-session -- xvfb-run -a sh -eu -c '
    openbox > "$2/wm.log" 2>&1 &
    wm=$!
    trap "kill $wm" EXIT
    for attempt in $(seq 1 100); do
        xprop -root _NET_SUPPORTING_WM_CHECK | grep -q "window id" && break
        sleep 0.1
    done
    "$1" -- "$2/folder with spaces" > "$2/app.log" 2>&1 &
    app=$!
    window=""
    for attempt in $(seq 1 300); do
        window=$(xdotool search --onlyvisible --pid "$app" 2>/dev/null | head -1) || true
        [ -z "$window" ] || break
        sleep 0.1
    done
    [ -n "$window" ]
    xdotool windowactivate --sync "$window" key --clearmodifiers alt+F4
    wait "$app"
' sh "$binary" "$work"
jq -e --arg path "$work/folder with spaces" \
    '(.sessions | length) == 1 and .sessions[0].path == $path' \
    "$work/data/mugi-grep/settings.json"
echo 'PASS: directory argument reaches the GUI; spaces and restored sessions handled.'
