# bs-fastify

🚧

Bucklescript bindings for [fastify](https://www.fastify.io/)

## Install

```
yarn add @dck/bs-node @dck/bs-fastify
```

## Usage

```ocaml
open BsFastify;
open BsNode;

let app = createApp(appOptions(~logger=true, ()));

app
|> get(baseEndpoint, (_req, res) =>
  res->Response.sendObject({"downloads": downloads^})
);


let schemaProperties = Js.Dict.empty();
schemaProperties->Js.Dict.set("name", Schema.property(~type_="string"));

app
  |> postWithSchema(
       baseEndpoint,
       Schema.createBodySchema(
         ~required=[|"name"|],
         ~properties=schemaProperties,
       ),
       (req, res) => {
         let name = req->Request.body##name;
         res->Response.sendString("Hello "++ name ++ " !");
       }
  );

app
|> listen(3333, (err, address) =>
  switch (Js.Nullable.toOption(err)) {
    | None => Js.log("Server listening on " ++ address)
    | Some(error) => Js.log(error)
  }
);
```
