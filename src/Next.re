// These are standard Node.js Request and Response objects, but we wanted to
// allow other libraries to wrap those APIs, and didn't want to bloat the size
// of this library by including one. If you want to do more with the request
// than get the headers, or if you want to do something special with the
// response, we suggest tasteful use of Obj.magic to convert these values to the
// type of choice that fits with the Node.js library you are using.
type request = {headers: Js.Dict.t(string)};
type response;

type context = {
  // Current route. That is the path of the page in /pages
  pathname: string,
  // Query string section of URL parsed as an object
  query: Js.Dict.t(string),
  // String of the actual path (including the query) shown in the browser
  asPath: string,
  // HTTP request object (server only)
  req: request,
  // HTTP response object (server only)
  res: response,
  // Error object if any error is encountered during the rendering
  err: Js.Exn.t,
};

[@bs.set]
external assignGetInitialProps:
  (React.component('props), context => Js.Promise.t('props)) => unit =
  "getInitialProps";

//
// React Components
//
module Head = {
  [@react.component] [@bs.module "next/head"]
  external make: (~children: React.element) => React.element = "default";
};

module Link = {
  [@react.component] [@bs.module "next/link"]
  external make:
    (
      ~children: React.element,
      ~href: string,
      ~as_: string=?,
      ~passHref: bool=?,
      ~prefetch: bool=?,
      ~shallow: bool=?,
      ~replace: bool=?,
      ~scroll: bool=?
    ) =>
    React.element =
    "default";
};

//
// Hooks
//

module Router = {
  type t = {
    // Current route. That is the path of the page in /pages
    pathname: string,
    // The query string parsed to an object. Defaults to {}
    query: Js.Dict.t(string),
    // Actual path (including the query) shown in the browser
    asPath: string,
  };

  type pushOptions = {
    shallow: bool,
    getInitialProps: option(bool),
  };
  [@bs.send]
  external push:
    (t, ~url: string, ~asUrl: string=?, ~options: pushOptions=?, unit) => unit =
    "push";

  [@bs.send]
  external replace:
    (t, ~url: string, ~asUrl: string=?, ~options: pushOptions=?, unit) => unit =
    "replace";

  type popStateContext = {
    url: string,
    as_: string,
    options: pushOptions,
  };
  [@bs.send]
  external beforePopState: (t, popStateContext => bool) => unit =
    "beforePopState";

  ();
  // Events are not wrapped at the moment. Feel free to contribute!
};

[@bs.module "next/router"] external useRouter: unit => Router.t = "useRouter";