type request = {headers: Js.Dict.t(string)};

type propsFetcherParams = {
  // This is an inexhaustive list of properties available.
  query: Js.Dict.t(string),
  pathname: string,
  asPath: string,
  // Defined only if the function is running in a server environment.
  req: option(request),
};

type propsFetcher('props) = propsFetcherParams => Js.Promise.t('props);

[@bs.set]
external assignPropsFetcher:
  (React.component('propsIn), propsFetcher('propsOut)) => unit =
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
      ~_as: string=?,
      ~replace: bool=?,
      ~scroll: bool=?,
      ~shallow: bool=?,
      ~passHref: bool=?,
      ~prefetch: bool=?
    ) =>
    React.element =
    "default";
};
