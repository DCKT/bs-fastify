open BsNode;
open Belt;
type t;

module Schema = {
  [@bs.deriving abstract]
  type property = {
    [@bs.as "type"]
    type_: string,
  };

  [@bs.deriving abstract]
  type bodySchemaConfig = {
    [@bs.as "type"]
    type_: string,
    [@bs.optional]
    required: array(string),
    properties: Js.Dict.t(property),
  };

  [@bs.deriving abstract]
  type schemaType = {
    [@bs.optional]
    body: bodySchemaConfig,
  };

  [@bs.deriving abstract]
  type t = {schema: schemaType};

  let createBodySchema =
      (
        ~required: option(array(string))=?,
        ~properties: Js.Dict.t(property),
      ) =>
    t(
      ~schema=
        schemaType(
          ~body=
            bodySchemaConfig(
              ~type_="object",
              ~required=required->Option.getWithDefault([||]),
              ~properties,
              (),
            ),
          (),
        ),
    );
};

[@bs.deriving abstract]
type appOptions = {
  [@bs.optional]
  logger: bool,
};

module Request = {
  type t;
  type log;

  module Headers = {
    type t;

    [@bs.get] external range: t => Js.Nullable.t(string) = "range";
  };

  module Log = {
    type t;

    [@bs.send] external info: (t, string) => unit = "info";
  };

  [@bs.get] external body: t => 'a = "body";
  [@bs.get] external params: t => 'a = "params";
  [@bs.get] external log: t => Log.t = "log";
  [@bs.get] external headers: t => Headers.t = "headers";

  [@bs.send]
  external multipart:
    (
      t,
      (string, Fs.Stream.t, string, string, string) => unit,
      Js.Nullable.t(Js.Exn.t) => unit
    ) =>
    unit =
    "multipart";
};

module Response = {
  type t;

  [@bs.send] external sendString: (t, string) => unit = "send";

  [@bs.send] external sendObject: (t, Js.t('a)) => unit = "send";

  [@bs.send] external sendUnit: (t, unit) => unit = "send";

  [@bs.send] external sendStream: (t, Fs.Stream.t) => unit = "send";

  [@bs.send] external code: (t, int) => unit = "code";

  [@bs.send] external header: (t, string, string) => unit = "header";
};

[@bs.module] external createApp: appOptions => t = "fastify";

[@bs.send.pipe: t] external register: 'a => unit = "register";

[@bs.send.pipe: t] external use: (string, 'a) => unit = "use";

[@bs.send.pipe: t]
external get: (string, (Request.t, Response.t) => unit) => unit = "get";

[@bs.send.pipe: t]
external post: (string, (Request.t, Response.t) => unit) => unit = "post";

[@bs.send.pipe: t]
external put: (string, (Request.t, Response.t) => unit) => unit = "put";

[@bs.send.pipe: t]
external delete: (string, (Request.t, Response.t) => unit) => unit = "delete";

[@bs.send.pipe: t]
external postWithSchema:
  (string, Schema.t, (Request.t, Response.t) => unit) => unit =
  "post";

[@bs.send.pipe: t]
external deleteWithSchema:
  (string, Schema.t, (Request.t, Response.t) => unit) => unit =
  "delete";

[@bs.send.pipe: t]
external listen: (int, (Js.Nullable.t(string), string) => unit) => unit =
  "listen";