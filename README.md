## Next.js, but in ReasonML and Bucklescript!

Type definitions for Next.js.

State:

- Incomplete type definitions. Feel free to open a pull-request to add more!
- Though incomplete, these typedefs are being used in production, so what's there does work.
- Wraps API for Next version 9.x.x on NPM
- Only works with Bucklescript 6.x and above.

# The Basics.

0. Have a reason project already set up

1. Install:

```
npm install --save next@9 nextdotbs
```

(Notice how you installed both next and bs-next!)

2. Add "bs-next" to your "bs-dependencies" in your "bsconfig.json"

3. [Install ReasonReact](https://reasonml.github.io/reason-react/docs/en/installation)

4. `mkdir pages`

5. Make a file: `src/pages/WelcomePage.re` **that defines a React component named default**.

```
open React;
open Next;

[@react.component]
let default = () => {
  <div>
    <Head>
      <title> "Welcome"->string </title>
    </Head>
    "Welcome to Next"->string
  </div>;
};
```

6. Symlink that file into the pages directory:

`cd pages && ln -s ../src/pages/WelcomePage.bs.js welcome.js`

The symlinking allows us to still use the .bs.js postfix for Reason files, and also allows us to use dynamic routing and name files with square brackets inside of the `pages` directory without upsetting `bsb`.

7. Launch Next and open the browser, etc. as explained in [the Next.js docs](https://nextjs.org/docs)

## Data Fetching

Next performs data fetching for page components by looking for a static function on the component class called `getInitialProps`. This isn't a pattern that ReasonReact likes, so instead, these wrappings expose a function called `assignPropsFetcher` that you pass your `default` value (from creating the React component) to:

```reason
open React;
open Next;

[@react.component]
let default = (~message: string) => {
  <div>
    <Head>
      <title> "Welcome"->string </title>
    </Head>
    "Welcome to Next"->string
  </div>;
};

// Notice how I match the props for the component above!
type props = {message: option(Post.t)};

let fetcher: propsFetcher(props) =
  ({req, query}) => {
    // ... do some async stuff we don't show here and get a value called "message" in scope. 🧙‍♂️
    {message: message}->Js.Promise.resolve;
  };

default->assignPropsFetcher(fetcher);
```

That's most of it! So to re-cap, you can use it just like Next.js except:

- Symlink your compiled Reason artifacts into the correct location in the `pages` folder.
- Use `assignPropsFetcher` instead of `getInitialProps`.
- The `req` param that gets passed into the props fetcher will be `None` on the client, but will be an object with a `headers` field on the server. If you're making a request from your own server to your own API to populate data, you can get the `host` and use it as the host for your HTTP request.

Other things to note:

- A `Head` React component is available for inserting tags into the head of the document.
- A `Link` React component is availalbe for slick in-app navigation.

## Usage with Serbet.

You may want to run Next alongside your own API, and you may be using [Serbet](https://github.com/mrmurphy/serbet) for that. Here's how they can work together:

```reason
open Async;

let nextServer =
  NextServer.make({
    dev:
      !
        Node.Process.process##env
        ->Js.Dict.get("NODE_ENV")
        ->Belt.Option.map(a => a == "production")
        ->Belt.Option.getWithDefault(false),
  });

{
  let%Async _ = nextServer->NextJSServer.prepare;
  let app =
    Serbet.application([
      // your API routes here
    ]);
  app.router->Express.Router.use(nextServer->NextServer.getRequestHandler);
  async();
};
```
