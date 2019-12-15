open BsFastify;

let app = createApp(AppOptions.make());

app->get("/get/unit", (_, res) => Response.(res->sendUnit()));
app->get("/get/string", (_, res) => Response.(res->sendString("test")));
app
->get("/get/:id", (req, res) =>
    Response.(res->sendString("Hello id " ++ Request.(req->params##"id")))
  );
app
->get("/get/object", (_, res) =>
    Response.(res->sendObject({"test": "test"}))
  );

app
->listen(3333, (err, address) =>
    switch (err->Js.Nullable.toOption) {
    | None => Js.log("Server ready on " ++ address)
    | Some(err) => Js.log2("An error occured\n", err)
    }
  );