FROM alpine:latest
RUN apk add --no-cache gcompat libstdc++

COPY --chmod=755 ./linux-x64-release/iw4-validator /usr/local/bin/

RUN addgroup -S iw4 && adduser -S iw4 -G iw4
USER iw4

ENTRYPOINT ["/usr/local/bin/iw4-validator"]
