#!/usr/bin/env bash

set -u

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ATHENA_BUILD_DIR:-${PROJECT_ROOT}/build}"
ATHENA_BIN="${BUILD_DIR}/athena"
REST_TEST_BIN="${BUILD_DIR}/athena_rest_api_tests"

SCHEME="http"
BASE_URL="${ATHENA_TEST_BASE_URL:-${SCHEME}://127.0.0.1:8848}"
HEALTH_URL="${BASE_URL}/api/v1/health"

SERVER_LOG="${BUILD_DIR}/athena-rest-test.log"
ATHENA_PID=""

cleanup()
{
    if [[ -n "${ATHENA_PID}" ]] &&
       kill -0 "${ATHENA_PID}" 2>/dev/null
    then
        echo "Stopping Athena process ${ATHENA_PID}"
        kill -INT "${ATHENA_PID}" 2>/dev/null || true
        wait "${ATHENA_PID}" 2>/dev/null || true
    fi
}

trap cleanup EXIT INT TERM

if [[ ! -x "${ATHENA_BIN}" ]]
then
    echo "Athena executable not found: ${ATHENA_BIN}" >&2
    echo "Build the project before running this script." >&2
    exit 1
fi

if [[ ! -x "${REST_TEST_BIN}" ]]
then
    echo "REST test executable not found: ${REST_TEST_BIN}" >&2
    echo "Build the project before running this script." >&2
    exit 1
fi

echo "Starting Athena"
"${ATHENA_BIN}" >"${SERVER_LOG}" 2>&1 &
ATHENA_PID=$!

echo "Waiting for Athena health endpoint"

READY=false

for attempt in {1..50}
do
    if ! kill -0 "${ATHENA_PID}" 2>/dev/null
    then
        echo "Athena stopped before becoming ready." >&2
        cat "${SERVER_LOG}" >&2
        exit 1
    fi

    if curl \
        --silent \
        --fail \
        --max-time 1 \
        "${HEALTH_URL}" \
        >/dev/null
    then
        READY=true
        break
    fi

    sleep 0.1
done

if [[ "${READY}" != true ]]
then
    echo "Athena did not become ready within five seconds." >&2
    cat "${SERVER_LOG}" >&2
    exit 1
fi

echo "Athena is ready"
echo "Running REST integration tests"

ATHENA_TEST_BASE_URL="${BASE_URL}" \
    "${REST_TEST_BIN}"

TEST_STATUS=$?

if [[ "${TEST_STATUS}" -ne 0 ]]
then
    echo "REST integration tests failed." >&2
    echo "Athena server log:" >&2
    cat "${SERVER_LOG}" >&2
    exit "${TEST_STATUS}"
fi

echo "REST integration tests passed"