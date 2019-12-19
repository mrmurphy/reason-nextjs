// These are the pieces you need if you want to add Next.js to an existing Express server.

type server;
type opts = {dev: bool};

// First, make a Next app.
[@bs.module] external make: opts => server = "next";

// Then prepare it, and after the preparation is done, you can start your express app.
[@bs.send] external prepare: server => Js.Promise.t(unit) = "prepare";

// This will return a middleware that you can pass to Express's "App.use" or "Router.use".
[@bs.send]
external getRequestHandler: server => 'expressMiddleware = "getRequestHandler";